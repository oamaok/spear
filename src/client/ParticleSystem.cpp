#include "ParticleSystem.h"

#include "client/AreaSystem.h"

#include "sf/HashMap.h"
#include "sf/Frustum.h"
#include "sf/Random.h"
#include "sf/Float4.h"

#include "sp/Renderer.h"

#include "client/ParticleTexture.h"
#include "client/BSpline.h"

#include "game/shader/GameShaders.h"
#include "game/shader/Particle.h"

#include "sp/Srgb.h"

namespace cl {

static const constexpr uint32_t MaxParticlesPerFrame = 4*1024;
static const constexpr uint32_t MaxParticleCacheFrames = 16;

static const constexpr float HugeParticleLife = 1e20f;
static const constexpr float HugeParticleLifeCmp = 1e19f;

struct ParticleSystemImp final : ParticleSystem
{
	struct EffectType
	{
		sf::Box<sv::ParticleSystemComponent> svComponent;

		uint32_t refCount = 0;
		float updateRadius;
		float timeStep;

		// TODO: Cache this?
		Particle_VertexType_t typeUbo;

		ParticleTextureRef texture;
	};

	struct Particle4
	{
		sf::Float4 px, py, pz;
		sf::Float4 vx, vy, vz;
		sf::Float4 life;
		sf::Float4 seed;
	};

	struct GpuParticle
	{
		sf::Vec3 position;
		float life;
		sf::Vec3 velocity;
		float seed;
	};

	struct Effect
	{
		uint32_t typeId;
		uint32_t areaId;

		sf::Bounds3 gpuBounds;
		float timeDelta = 0.0f;
		float timeStep;

		sf::Mat34 emitterToWorld;
		sf::Vec3 gravity;

		float spawnTimer = 0.0f;

		uint32_t uploadByteOffset = 0;
		uint64_t uploadFrameIndex = MaxParticleCacheFrames;
		sf::Array<Particle4> particles; // TODO: Alignment
		sf::Array<GpuParticle> gpuParticles;
		sf::Array<uint32_t> freeIndices;
	};

	struct ParticleUpdateCtx
	{
		sf::Random rng;
	};

	sf::Array<Effect> effects;
	sf::Array<uint32_t> freeEffectIds;

	sf::Array<EffectType> types;
	sf::Array<uint32_t> freeTypeIds;
	sf::HashMap<uintptr_t, uint32_t> svCompponentToType;

	sf::Random initRng;
	ParticleUpdateCtx updateCtx;

	static constexpr const uint32_t SplineSampleRate = 64;
	static constexpr const uint32_t SplineAtlasWidth = 8;
	static constexpr const uint32_t SplineAtlasHeight = 256;
	static constexpr const uint32_t SplineCountPerType = 2;
	static constexpr const uint32_t SplineAtlasResX = SplineSampleRate * SplineAtlasWidth;
	static constexpr const uint32_t SplineAtlasResY = SplineAtlasHeight * SplineCountPerType;
	sp::Texture splineTexture;

	sp::Buffer vertexBuffers[MaxParticleCacheFrames];
	sp::Pipeline particlePipe;
	uint64_t bufferFrameIndex = MaxParticleCacheFrames;

	ParticleSystemImp(const SystemsDesc &desc)
	{
		updateCtx.rng = sf::Random(desc.seed[1], 581271);

		for (uint32_t i = 0; i < MaxParticleCacheFrames; i++) {
			sf::SmallStringBuf<128> name;
			name.format("particle vertexBuffer %u", i);
			vertexBuffers[i].initDynamicVertex(name.data, sizeof(GpuParticle) * 4 * MaxParticlesPerFrame);
		}

		{
			uint32_t flags = sp::PipeIndex16|sp::PipeDepthTest|sp::PipeBlendPremultiply;
			sg_pipeline_desc &d = particlePipe.init(gameShaders.particle, flags);
			d.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT4;
			d.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
		}

		{
			sg_image_desc d = { };
			d.usage = SG_USAGE_IMMUTABLE;
			d.bqq_copy_target = true;
			d.pixel_format = SG_PIXELFORMAT_RGBA8;
			d.mag_filter = SG_FILTER_LINEAR;
			d.min_filter = SG_FILTER_LINEAR;
			d.width = SplineAtlasResX;
			d.height = SplineAtlasResY;
			d.label = "ParticleSystem splineTexture";
			splineTexture.init(d);
		}
	}

	void initEffectType(uint32_t typeId, const sv::ParticleSystemComponent &c)
	{
		EffectType &type = types[typeId];

		type.texture.load(c.sprite);
		type.updateRadius = c.updateRadius;
		type.timeStep = c.timeStep;

		sf::memZero(type.typeUbo);
		type.typeUbo.u_FrameCount = sf::Vec2(c.frameCount);
		type.typeUbo.u_FrameRate = c.frameRate;

		uint32_t atlasX = typeId / SplineCountPerType;
		uint32_t atlasY = typeId % SplineCountPerType;

		uint8_t splineTex[SplineSampleRate * 4 * 2];
		float splineY[SplineSampleRate];

		if (c.scaleSpline.points.size >= 1) {
			cl::discretizeBSplineY(splineY, c.scaleSpline.points, 0.0f, 1.0f);
		} else {
			for (float &v : splineY) v = 1.0f;
		}
		for (uint32_t i = 0; i < SplineSampleRate; i++) {
			splineTex[i * 4 + 0] = (uint8_t)sf::clamp((int)(splineY[i] * 255.0f), 0, 255);
		}

		if (c.alphaSpline.points.size >= 1) {
			cl::discretizeBSplineY(splineY, c.alphaSpline.points, 0.0f, 1.0f);
		} else {
			for (float &v : splineY) v = 1.0f;
		}
		for (uint32_t i = 0; i < SplineSampleRate; i++) {
			splineTex[i * 4 + 1] = (uint8_t)sf::clamp((int)(splineY[i] * 255.0f), 0, 255);
		}

		if (c.additiveSpline.points.size >= 1) {
			cl::discretizeBSplineY(splineY, c.additiveSpline.points, 0.0f, 1.0f);
			for (float &v : splineY) v = 1.0f - v;
		} else {
			for (float &v : splineY) v = 1.0f;
		}
		for (uint32_t i = 0; i < SplineSampleRate; i++) {
			splineTex[i * 4 + 2] = (uint8_t)sf::clamp((int)(splineY[i] * 255.0f), 0, 255);
		}

		for (uint32_t i = 0; i < SplineSampleRate; i++) {
			splineTex[i * 4 + 3] = 0;
		}

		uint8_t *gradientTex = splineTex + SplineSampleRate * 4;
		sv::GradientPoint defaultGradient[] = {
			{ 1.0f, c.gradient.defaultColor },
		};

		{
			sf::Slice<const sv::GradientPoint> points = c.gradient.points;
			if (points.size == 0) {
				points = defaultGradient;
			}

			float splineStep = 1.0f / (SplineSampleRate - 1);
			uint32_t pointIx = 0;
			for (uint32_t i = 0; i < SplineSampleRate; i++) {
				float t = (float)i * splineStep;

				while (pointIx < points.size && t >= points[pointIx].t) {
					pointIx++;
				}

				sf::Vec3 col;
				if (pointIx == 0) {
					col = points[0].color;
				} else if (pointIx == points.size) {
					col = points[points.size - 1].color;
				} else {
					const sv::GradientPoint &p0 = points[pointIx - 1];
					const sv::GradientPoint &p1 = points[pointIx];
					float relT = (t - p0.t) / (p1.t - p0.t);
					col = sf::lerp(p0.color, p1.color, relT);
				}

				uint8_t *dst = gradientTex + i * 4;
				sp::linearToSrgbUnorm(dst, col);
			}
		}

		uint32_t x = atlasX * SplineSampleRate;
		uint32_t y = atlasY * SplineCountPerType;

		sf::Vec2 atlasRes = sf::Vec2((float)SplineAtlasResX, (float)SplineAtlasResY);
		sf::Vec2 uvBias = sf::Vec2((float)x + 0.5f, (float)y + 0.5f) / atlasRes;
		sf::Vec2 uvScale = sf::Vec2((float)(SplineSampleRate - 1), 1.0f) / atlasRes;

		type.typeUbo.u_SplineMad.x = uvScale.x;
		type.typeUbo.u_SplineMad.y = uvScale.y;
		type.typeUbo.u_SplineMad.z = uvBias.x;
		type.typeUbo.u_SplineMad.w = uvBias.y;

		type.typeUbo.u_ScaleBaseVariance.x = c.size;
		type.typeUbo.u_ScaleBaseVariance.y = c.sizeVariance;

		sg_image_desc d = { };
		d.width = SplineSampleRate;
		d.height = SplineCountPerType;
		d.pixel_format = SG_PIXELFORMAT_RGBA8;
		d.content.subimage[0][0].ptr = splineTex;
		d.content.subimage[0][0].size = sizeof(splineTex);
		sg_image staging = sg_make_image(&d);

		{
			sg_bqq_subimage_rect rect;
			rect.src_x = 0;
			rect.src_y = 0;
			rect.dst_x = (int)x;
			rect.dst_y = (int)y;
			rect.dst_z = 0;
			rect.width = (int)SplineSampleRate;
			rect.height = (int)SplineCountPerType;

			sg_bqq_subimage_copy_desc desc;
			desc.dst_image = splineTexture.image;
			desc.src_image = staging;
			desc.rects = &rect;
			desc.num_rects = 1;
			desc.num_mips = 1;

			sg_bqq_copy_subimages(&desc);
		}

		sg_destroy_image(staging);
	}

	void simulateParticlesImp(sf::Random &rng, Effect &effect, float dt)
	{
		EffectType &type = types[effect.typeId];
		const sv::ParticleSystemComponent &comp = *type.svComponent;

		sf::ScalarFloat4 dt4 = dt;
		sf::ScalarFloat4 drag4 = comp.drag;

		// Update spawning (scalar)
		effect.spawnTimer -= dt;
		int spawnsLeft = 10;
		while (effect.spawnTimer <= 0.0f) {
			if (spawnsLeft-- <= 0) break;
			effect.spawnTimer += comp.spawnTime + comp.spawnTimeVariance * rng.nextFloat();

			if (effect.freeIndices.size == 0) {
				uint32_t base = effect.particles.size * 4;
				Particle4 &parts = effect.particles.push();
				parts.life = HugeParticleLife;
				for (uint32_t i = 0; i < 4; i++) {
					effect.freeIndices.push(base + i);
				}
			}

			uint32_t index = effect.freeIndices.popValue();

			sf::Vec3 pos = comp.emitOrigin;
			sf::Vec3 vel = sf::Vec3(0.0f);

			pos = sf::transformPoint(effect.emitterToWorld, pos);

			Particle4 &p = effect.particles[index >> 2];
			uint32_t lane = index & 3;
			sf::setLaneInMemory(p.px, lane, pos.x);
			sf::setLaneInMemory(p.py, lane, pos.y);
			sf::setLaneInMemory(p.pz, lane, pos.z);
			sf::setLaneInMemory(p.vx, lane, vel.x);
			sf::setLaneInMemory(p.vy, lane, vel.y);
			sf::setLaneInMemory(p.vz, lane, vel.z);
			sf::setLaneInMemory(p.life, lane, 1.0f);
			sf::setLaneInMemory(p.seed, lane, rng.nextFloat() * 16777216.0f);
		}
		effect.spawnTimer = sf::max(effect.spawnTimer, 0.0f);

		// Integration
		size_t numGpuParticles = effect.particles.size * (4 * 4);
		effect.gpuParticles.reserveGeometric(numGpuParticles);
		effect.gpuParticles.resizeUninit(numGpuParticles);
		GpuParticle *dst = effect.gpuParticles.data;

		sf::Float4 pMin = +HUGE_VALF, pMax = -HUGE_VALF;

		for (Particle4 &p : effect.particles) {

			sf::Float4 life = p.life;

			if (!life.allGreaterThanZero()) {
				uint32_t base = (uint32_t)(&p - effect.particles.data) * 4;
				float lifes[4];
				life.storeu(lifes);
				for (uint32_t i = 0; i < 4; i++) {
					if (lifes[i] <= 0.0f) {
						effect.freeIndices.push(base + i);
						lifes[i] = HugeParticleLife;
					}
				}
				life = sf::Float4::loadu(lifes);
			}

			life -= dt4;
			p.life = life;

			sf::Float4 px = p.px, py = p.py, pz = p.pz;
			sf::Float4 vx = p.vx, vy = p.vy, vz = p.vz;

			sf::Float4 ax = effect.gravity.x;
			sf::Float4 ay = effect.gravity.y;
			sf::Float4 az = effect.gravity.z;
			ax -= vx * drag4;
			ay -= vy * drag4;
			az -= vz * drag4;

			vx += ax * dt4;
			vy += ay * dt4;
			vz += az * dt4;

			p.vx = vx; p.vy = vy; p.vz = vz;

			px += vx * dt4;
			py += vy * dt4;
			pz += vz * dt4;

			p.px = px; p.py = py; p.pz = pz;

			sf::Float4::transpose4(px, py, pz, life);

			sf::Float4 seed = p.seed;
			sf::Float4::transpose4(vx, vy, vz, seed);

			if (px.getW() < HugeParticleLifeCmp) {
				px.storeu((float*)&dst[0] + 0);
				vx.storeu((float*)&dst[0] + 4);
				px.storeu((float*)&dst[1] + 0);
				vx.storeu((float*)&dst[1] + 4);
				px.storeu((float*)&dst[2] + 0);
				vx.storeu((float*)&dst[2] + 4);
				px.storeu((float*)&dst[3] + 0);
				vx.storeu((float*)&dst[3] + 4);
				pMin = pMin.min(px);
				pMax = pMax.max(px);
				dst += 4;
			}

			if (py.getW() < HugeParticleLifeCmp) {
				py.storeu((float*)&dst[0] + 0);
				vy.storeu((float*)&dst[0] + 4);
				py.storeu((float*)&dst[1] + 0);
				vy.storeu((float*)&dst[1] + 4);
				py.storeu((float*)&dst[2] + 0);
				vy.storeu((float*)&dst[2] + 4);
				py.storeu((float*)&dst[3] + 0);
				vy.storeu((float*)&dst[3] + 4);
				pMin = pMin.min(py);
				pMax = pMax.max(py);
				dst += 4;
			}

			if (pz.getW() < HugeParticleLifeCmp) {
				pz.storeu((float*)&dst[0] + 0);
				vz.storeu((float*)&dst[0] + 4);
				pz.storeu((float*)&dst[1] + 0);
				vz.storeu((float*)&dst[1] + 4);
				pz.storeu((float*)&dst[2] + 0);
				vz.storeu((float*)&dst[2] + 4);
				pz.storeu((float*)&dst[3] + 0);
				vz.storeu((float*)&dst[3] + 4);
				pMin = pMin.min(pz);
				pMax = pMax.max(pz);
				dst += 4;
			}

			if (life.getW() < HugeParticleLifeCmp) {
				life.storeu((float*)&dst[0] + 0);
				seed.storeu((float*)&dst[0] + 4);
				life.storeu((float*)&dst[1] + 0);
				seed.storeu((float*)&dst[1] + 4);
				life.storeu((float*)&dst[2] + 0);
				seed.storeu((float*)&dst[2] + 4);
				life.storeu((float*)&dst[3] + 0);
				seed.storeu((float*)&dst[3] + 4);
				pMin = pMin.min(life);
				pMax = pMax.max(life);
				dst += 4;
			}
		}

		effect.gpuParticles.resizeUninit(dst - effect.gpuParticles.data);

		effect.gpuBounds.origin = ((pMin + pMax) * 0.5f).asVec3();
		effect.gpuBounds.extent = ((pMax - pMin) * 0.5f + comp.cullPadding).asVec3();
		effect.uploadFrameIndex = 0;
	}

	// API

	virtual void addEffect(Systems &systems, uint32_t entityId, uint8_t componentIndex, const sf::Box<sv::ParticleSystemComponent> &c, const Transform &transform) override
	{
		uint32_t effectId = effects.size;
		if (freeEffectIds.size > 0) {
			effectId = freeEffectIds.popValue();
		} else {
			effects.push();
		}

		uint32_t typeId;
		uintptr_t key = (uintptr_t)c.ptr;
		auto res = svCompponentToType.insert(key);
		if (res.inserted) {
			typeId = types.size;
			if (freeTypeIds.size > 0) {
				typeId = freeTypeIds.popValue();
			} else {
				types.push();
			}

			types[typeId].svComponent = c;
			initEffectType(typeId, *c);
		} else {
			typeId = res.entry.val;
		}

		EffectType &type = types[typeId];
		type.refCount++;

		Effect &effect = effects[effectId];
		effect.typeId = typeId;
		effect.emitterToWorld = transform.asMatrix();
		effect.timeStep = type.timeStep * (1.0f + initRng.nextFloat() * 0.1f);

		sf::Bounds3 bounds;
		bounds.origin = transform.position;
		bounds.extent = sf::Vec3(c->updateRadius);
		effect.areaId = systems.area->addBoxArea(AreaGroup::ParticleEffect, effectId, bounds, Area::Visibilty);

		systems.entities.addComponent(entityId, this, effectId, 0, componentIndex, Entity::UpdateTransform);
	}

	void updateTransform(Systems &systems, uint32_t entityId, const EntityComponent &ec, const TransformUpdate &update) override
	{
		uint32_t effectId = ec.userId;
		Effect &effect = effects[effectId];
		EffectType &type = types[effect.typeId];

		effect.emitterToWorld = update.entityToWorld;

		sf::Bounds3 bounds;
		bounds.origin = update.transform.position;
		bounds.extent = sf::Vec3(type.updateRadius);
		systems.area->updateBoxArea(effect.areaId, bounds);
	}

	void remove(Systems &systems, uint32_t entityId, const EntityComponent &ec) override
	{
		uint32_t effectId = ec.userId;
		Effect &effect = effects[effectId];
		EffectType &type = types[effect.typeId];

		systems.area->removeBoxArea(effect.areaId);

		if (--type.refCount == 0) {
			uintptr_t key = (uintptr_t)type.svComponent.ptr;
			svCompponentToType.remove(key);

			sf::reset(type);
			freeTypeIds.push(effect.typeId);
		}

		sf::reset(effect);
		freeEffectIds.push(effectId);
	}

	void updateParticles(const VisibleAreas &visibleAreas, float dt) override
	{
		float clampedDt = sf::min(dt, 0.1f);

		for (uint32_t effectId : visibleAreas.get(AreaGroup::ParticleEffect)) {
			Effect &effect = effects[effectId];

			effect.timeDelta += clampedDt;
			while (effect.timeDelta >= effect.timeStep) {
				simulateParticlesImp(updateCtx.rng, effect, effect.timeStep);
				effect.timeDelta -= effect.timeStep;
			}
		}
	}

	void renderMain(const VisibleAreas &visibleAreas, const RenderArgs &renderArgs) override
	{
		// TODO: Sort by type
		uint32_t frameParticlesLeft = MaxParticlesPerFrame;

		bufferFrameIndex++;

		uint64_t frame = bufferFrameIndex;
		for (uint32_t effectId : visibleAreas.get(AreaGroup::ParticleEffect)) {
			Effect &effect = effects[effectId];
			EffectType &type = types[effect.typeId];
			uint32_t numParticles = effect.gpuParticles.size / 4;

			if (numParticles == 0) continue;
			if (!renderArgs.frustum.intersects(effect.gpuBounds)) continue;

			if (frame - effect.uploadFrameIndex >= MaxParticleCacheFrames) {
				if (numParticles > MaxParticlesPerFrame) return;

				frameParticlesLeft -= numParticles;
				effect.uploadByteOffset = (uint32_t)sg_append_buffer(vertexBuffers[(uint32_t)frame % MaxParticleCacheFrames].buffer, effect.gpuParticles.data, (int)effect.gpuParticles.byteSize());
				effect.uploadFrameIndex = frame;
			}

			sp::Buffer &vertexBuffer = vertexBuffers[(uint32_t)effect.uploadFrameIndex % MaxParticleCacheFrames];

			Particle_VertexInstance_t ubo;
			renderArgs.worldToClip.writeColMajor44(ubo.u_WorldToClip);
			ubo.u_Aspect = renderArgs.viewToClip.m00 / renderArgs.viewToClip.m11;
			ubo.u_InvDelta = effect.timeStep - effect.timeDelta;

			particlePipe.bind();

			sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_Particle_VertexType, &type.typeUbo, sizeof(type.typeUbo));
			sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_Particle_VertexInstance, &ubo, sizeof(ubo));

			sg_image image = ParticleTexture::defaultImage;
			if (type.texture.isLoaded() && type.texture->image.id) {
				image = type.texture->image;
			}

			sg_bindings binds = { };
			binds.vertex_buffers[0] = vertexBuffer.buffer;
			binds.vertex_buffer_offsets[0] = effect.uploadByteOffset;
			binds.index_buffer = sp::getSharedQuadIndexBuffer();
			binds.vs_images[SLOT_Particle_u_SplineTexture] = splineTexture.image;
			binds.fs_images[SLOT_Particle_u_Texture] = image;
			sg_apply_bindings(&binds);

			sg_draw(0, numParticles * 6, 1);
		}
	}

};

sf::Box<ParticleSystem> ParticleSystem::create(const SystemsDesc &desc) { return sf::box<ParticleSystemImp>(desc); }


}
