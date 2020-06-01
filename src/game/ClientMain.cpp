#include "ClientMain.h"

#include "game/client/ClientState.h"
#include "game/server/Message.h"

#include "MessageTransport.h"
#include "LocalServer.h"

#include "ext/bq_websocket.h"
#include "ext/bq_websocket_platform.h"
#include "ext/sokol/sokol_gfx.h"

#include "sp/Renderer.h"
#include "game/shader/GameShaders.h"
#include "game/shader/Postprocess.h"
#include "game/shader/Fxaa.h"
#include "game/client/TileMaterial.h"

#include "GameConfig.h"

// TEMP
#include "sf/Frustum.h"
#include "ext/sokol/sokol_time.h"
#include "game/shader2/GameShaders2.h"
#include "sp/Canvas.h"
#include "sp/Sprite.h"
#include "sp/Font.h"

static sf::Symbol serverName { "Server" };

static uint32_t playerIdCounter = 100;

struct CardGuiState
{
	float hover = 0.0f;
};

struct ClientMain
{
	bqws_socket *ws;
	sf::Symbol name;

	sf::Box<sv::State> serverState;
	cl::State clientState;
	uint32_t reloadCount = 0;

	sf::Vec2i resolution;

	sp::Pipeline tonemapPipe;
	sp::Pipeline fxaaPipe;

	sp::RenderTarget mainTarget;
	sp::RenderTarget mainDepth;
	sp::RenderTarget tonemapTarget;
	sp::RenderTarget fxaaTarget;

	sp::RenderPass mainPass;
	sp::RenderPass tonemapPass;
	sp::RenderPass fxaaPass;

	sf::Vec2 uiResolution;
	sp::Canvas testCanvas;
	sp::FontRef cardFont { sf::Symbol("Assets/Gui/Font/NotoSans-Regular.ttf") };
	sp::FontRef cardFontBold { sf::Symbol("Assets/Gui/Font/NotoSans-Bold.ttf") };
	sp::SpriteRef cardBackground { sf::Symbol("Assets/Gui/Card/Background_Base.png") };
	sf::Array<CardGuiState> cardGuiState;
	int32_t selectedCard = -1;
	int32_t hoveredCard = -1;
	float selectedOffset = 0.0f;

	Shader2 testMeshShader;
	Shader2 testSkinShader;

	sp::Pipeline tempMeshPipe;
	sp::Pipeline tempSkinnedMeshPipe;
};

void clientGlobalInit()
{
	cl::TileMaterial::globalInit();
}

void clientGlobalCleanup()
{
	cl::TileMaterial::globalCleanup();
}

static bool useNormalRemap(sg_pixel_format format)
{
	switch (format) {
	case SG_PIXELFORMAT_RG8:
	case SG_PIXELFORMAT_RGBA8:
	case SG_PIXELFORMAT_BC5_RG:
		return false;
	case SG_PIXELFORMAT_BC3_RGBA:
	case SG_PIXELFORMAT_BQQ_ASTC_4X4_RGBA:
	case SG_PIXELFORMAT_BQQ_ASTC_8X8_RGBA:
		return true;
	default:
		sf_failf("Unexpected format: %u", format);
		return false;
	}
}

ClientMain *clientInit(int port, const sf::Symbol &name)
{
	ClientMain *c = new ClientMain();
	c->name = name;

	gameShaders.load();

	{
        sf::SmallStringBuf<128> url;

#if defined(GAME_WEBSOCKET_URL)
        url = GAME_WEBSOCKET_URL;
		port = 80;
#else
        url.format("ws://localhost:%d", port);
#endif
        
		bqws_opts opts = { };
		opts.name = name.data;
		if (port > 0) {
			c->ws = bqws_pt_connect(url.data, NULL, &opts, NULL);
		} else {
			c->ws = localServerConnect(port, &opts, NULL);
		}
	}

	{
		sv::MessageJoin join;
		join.name = name;
		join.sessionId = 1;
		join.sessionSecret = 10;
		join.playerId = ++playerIdCounter;
		writeMessage(c->ws, &join, c->name, serverName);
	}

	c->tonemapPipe.init(gameShaders.postprocess, sp::PipeVertexFloat2);
	c->fxaaPipe.init(gameShaders.fxaa, sp::PipeVertexFloat2);

	sg_pixel_format normalFormat = (sg_pixel_format)cl::TileMaterial::getAtlasPixelFormat(cl::MaterialTexture::Normal);

	uint8_t permutation[SP_NUM_PERMUTATIONS] = { };
	#if SF_OS_WASM
		permutation[SP_SHADOWGRID_USE_ARRAY] = 1;
	#else
		permutation[SP_SHADOWGRID_USE_ARRAY] = 0;
	#endif
	permutation[SP_NORMALMAP_REMAP] = useNormalRemap(normalFormat);
	c->testMeshShader = getShader2(SpShader_TestMesh, permutation);
	c->testSkinShader = getShader2(SpShader_TestSkin, permutation);

	{
		uint32_t flags = sp::PipeDepthWrite|sp::PipeIndex16|sp::PipeCullCCW;
		auto &d = c->tempMeshPipe.init(c->testMeshShader.handle, flags);
		d.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		d.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT3;
		d.layout.attrs[2].format = SG_VERTEXFORMAT_FLOAT4;
		d.layout.attrs[3].format = SG_VERTEXFORMAT_FLOAT2;
	}

	{
		uint32_t flags = sp::PipeDepthWrite|sp::PipeIndex16|sp::PipeCullCCW;
		auto &d = c->tempSkinnedMeshPipe.init(c->testSkinShader.handle, flags);
		d.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		d.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT2;
		d.layout.attrs[2].format = SG_VERTEXFORMAT_SHORT4N;
		d.layout.attrs[3].format = SG_VERTEXFORMAT_SHORT4N;
		d.layout.attrs[4].format = SG_VERTEXFORMAT_UBYTE4;
		d.layout.attrs[5].format = SG_VERTEXFORMAT_UBYTE4N;
	}

	// HACK
	{
		cl::PointLight &l = c->clientState.pointLights.push();
		l.position = sf::Vec3(0.0f, 4.0f, 0.0f);
		l.color = sf::Vec3(4.0f, 0.0f, 0.0f) * 2.0f;
		l.radius = 16.0f;
		l.shadowIndex = 0;
	}

	{
		cl::PointLight &l = c->clientState.pointLights.push();
		l.position = sf::Vec3(0.0f, 4.0f, 0.0f);
		l.color = sf::Vec3(0.0f, 3.0f, 0.0f) * 2.0f;
		l.radius = 16.0f;
		l.shadowIndex = 1;
	}

	{
		cl::PointLight &l = c->clientState.pointLights.push();
		l.position = sf::Vec3(0.0f, 4.0f, 0.0f);
		l.color = sf::Vec3(0.0f, 0.0f, 6.0f) * 2.0f;
		l.radius = 16.0f;
		l.shadowIndex = 2;
	}

	{
		cl::PointLight &l = c->clientState.pointLights.push();
		l.position = sf::Vec3(0.0f, 2.0f, 0.0f);
		l.color = sf::Vec3(4.0f, 4.0f, 4.0f) * 2.0f;
		l.radius = 16.0f;
		l.shadowIndex = 3;
	}

	return c;
}

void clientQuit(ClientMain *client)
{
	bqws_close(client->ws, BQWS_CLOSE_NORMAL, NULL, 0);
}

static void recreateTargets(ClientMain *c, const sf::Vec2i &systemRes)
{
	c->resolution = systemRes;

	float scale = 1.0f;
	sf::Vec2i mainRes = sf::Vec2i(sf::Vec2(systemRes) * sqrtf(scale));

	int mainSamples = 1;
	sg_pixel_format mainFormat = SG_PIXELFORMAT_RGBA8;
	sg_pixel_format mainDepthFormat = SG_PIXELFORMAT_DEPTH_STENCIL;

	c->mainTarget.init("mainTarget", mainRes, mainFormat, mainSamples);
	c->mainDepth.init("mainDepth", mainRes, mainDepthFormat, mainSamples);
	c->tonemapTarget.init("tonemapTarget", mainRes, SG_PIXELFORMAT_RGBA8);
	c->fxaaTarget.init("fxaaTarget", mainRes, SG_PIXELFORMAT_RGBA8);

	c->mainPass.init("main", c->mainTarget, c->mainDepth);
	c->tonemapPass.init("tonemap", c->tonemapTarget);
	c->fxaaPass.init("fxaa", c->fxaaTarget);

	c->clientState.recreateTargets();
}

struct RichTextDesc
{
	float wrapWidth;
	float fontHeight;
	sp::Font *bodyFont;
	sp::Font *boldFont;
	sf::Vec4 bodyColor;
	sf::Vec4 boldColor;
};

struct RichTextDraw
{
	bool newLine;
	bool newParagraph;
	sf::String text;
	sp::Font *font;
	sf::Vec4 color;
	float fontHeight;
};

static float drawRichText(sp::Canvas &canvas, const RichTextDesc &desc, sf::Slice<const sf::String> paragraphs)
{
	sf::SmallArray<RichTextDraw, 64> draws;
	for (sf::String text : paragraphs) {

		bool newParagraph = true;
		bool newLine = true;
		bool isBold = false;
		size_t begin = 0, end = 0;
		bool reachedEnd = false;
		while (!reachedEnd) {
			bool flush = true;
			bool nextNewLine = false;
			bool nextIsBold = isBold;
			switch (end < text.size ? text.data[end] : '\0') {

			case '\0':
				reachedEnd = true;
				break;

			case '*':
				nextIsBold = !isBold;
				break;

			case '\n':
				nextNewLine = true;
				break;

			default:
				flush = false;
				break;
			}
			end++;

			if (flush) {
				RichTextDraw &draw = draws.push();
				draw.text = sf::String(text.data + begin, end - begin - 1);
				draw.newLine = newLine;
				draw.newParagraph = newParagraph;
				draw.font = isBold ? desc.boldFont : desc.bodyFont;
				draw.color = isBold ? desc.boldColor : desc.bodyColor;
				draw.fontHeight = desc.fontHeight;

				newParagraph = 0;
				newLine = nextNewLine;
				isBold = nextIsBold;
				begin = end;
			}

		}
	}

	sf::Vec2 spaceSize = desc.bodyFont->measureText(" ", desc.fontHeight);

	sf::Vec2 pos = sf::Vec2(0.0f);
	for (const RichTextDraw &draw : draws) {

		if (draw.newParagraph) {
			pos.x = 0.0f;
			pos.y += desc.fontHeight * 1.5f;
		} else if (draw.newLine) {
			pos.x = 0.0f;
			pos.y += desc.fontHeight;
		}

		size_t begin = 0;
		size_t prevEnd = 0;
		size_t end = 0;

		float currentWidth = 0.0f;
		for (; end <= draw.text.size; end++) {

			if (end >= begin && end == draw.text.size || draw.text.data[end] == ' ') {
				sf::String piece { draw.text.data + prevEnd, end - prevEnd };
				float width = draw.font->measureText(piece, draw.fontHeight).x;

				if (pos.x + currentWidth + width < desc.wrapWidth) {
					currentWidth += width;
				} else {
					sf::String prevPiece { draw.text.data + begin, prevEnd - begin };

					sp::TextDraw td;
					td.string = prevPiece;
					td.transform.m02 = pos.x;
					td.transform.m12 = pos.y;
					td.font = draw.font;
					td.height = draw.fontHeight;
					td.color = draw.color;
					canvas.drawText(td);

					begin = prevEnd;
					pos.x = 0.0f;
					pos.y += desc.fontHeight;

					while (begin < draw.text.size && draw.text.data[begin] == ' ') {
						begin++;
					}
					sf::String nextPiece { draw.text.data + begin, end - begin };
					currentWidth = draw.font->measureText(nextPiece, draw.fontHeight).x;
				}
				prevEnd = end;

				if (end == draw.text.size && begin < end) {
					sf::String lastPiece { draw.text.data + begin, end - begin };

					sp::TextDraw td;
					td.string = lastPiece;
					td.transform.m02 = pos.x;
					td.transform.m12 = pos.y;
					td.font = draw.font;
					td.height = draw.fontHeight;
					td.color = draw.color;
					canvas.drawText(td);

					pos.x += currentWidth;
				}
			}
		}
	}

	return 0.0f;
}

static void drawCard(ClientMain *c, sp::Canvas &canvas, const cl::Card &card)
{
	sf::Vec2 size = { 100.0f, 100.0f / cl::Card::Aspect };

	canvas.draw(c->cardBackground, sf::Vec2(0.0f, 0.0f), size);

	{
		sf::Vec2 imageOffset = { 8.0f, 8.0f };
		sf::Vec2 imageSize = { 100.0f - 2.0f * 8.0f, 60.0f };
		sf::Vec2 spriteSize;
		if (card.imageSprite->aspect > imageSize.x / imageSize.y) {
			spriteSize.x = imageSize.x;
			spriteSize.y = imageSize.x / card.imageSprite->aspect;
		} else {
			spriteSize.y = imageSize.y;
			spriteSize.x = imageSize.y * card.imageSprite->aspect;
		}

		canvas.draw(card.imageSprite, imageOffset + imageSize * 0.5f - spriteSize * 0.5f, spriteSize);
	}

	{
		float titleHeight = 10.0f;
		{
			sf::Vec2 measure = c->cardFont->measureText(card.svCard.type->name, titleHeight);

			sp::TextDraw draw;
			draw.transform = sf::mat2D::translate(50.0f - measure.x/2.0f, 78.0f);
			draw.font = c->cardFont;
			draw.string = card.svCard.type->name;
			draw.height = titleHeight;
			draw.color = sf::Vec4(1.0f);
			canvas.drawText(draw);
		}

		sf::SmallArray<sf::String, 32> paragraphs;
		for (sf::Symbol &buf : card.svCard.type->description) {
			paragraphs.push(buf);
		}
		
		{
			sf::Mat23 bodyTransform;
			bodyTransform.m02 = 10.0f;
			bodyTransform.m12 = 80.0f;

			canvas.pushTransform(bodyTransform);

			RichTextDesc desc;
			desc.bodyFont = c->cardFont;
			desc.boldFont = c->cardFontBold;
			desc.wrapWidth = 80.0f;
			desc.fontHeight = 7.5f;
			desc.bodyColor = sf::Vec4(sf::Vec3(0.1f), 1.0f);
			desc.boldColor = sf::Vec4(sf::Vec3(0.0f, 0.0f, 0.0f), 1.0f);
			drawRichText(canvas, desc, paragraphs);

			canvas.popTransform();
		}
	}

}

static void lerpExp(float &state, float target, float exponential, float linear, float dt)
{
	state = sf::lerp(target, state, exp2f(dt*-exponential));

	float speed = linear * dt;
	state += sf::clamp(target - state, -speed, speed);
}

bool clientUpdate(ClientMain *c, const ClientInput &input)
{
	float dt = input.dt;
	if (bqws_is_closed(c->ws)) {
		return true;
	}
	if (input.resolution != c->resolution) {
		recreateTargets(c, input.resolution);
	}

	c->uiResolution.y = 720.0f;
	c->uiResolution.x = c->uiResolution.y * ((float)input.resolution.x / (float)input.resolution.y);

	bqws_update(c->ws);

	{
		uint32_t reloadCount = sp::Asset::getReloadCount();
		if (reloadCount != c->reloadCount) {
			c->reloadCount = reloadCount;
			c->clientState.assetsReloaded();
		}
	}

	c->clientState.updateMapChunks(*c->serverState);

	while (bqws_msg *wsMsg = bqws_recv(c->ws)) {
		sf::Box<sv::Message> msg = readMessage(wsMsg);
		sf_assert(msg);

		if (auto m = msg->as<sv::MessageLoad>()) {
			m->state->refreshEntityTileMap();
			c->serverState = m->state;
			c->clientState.reset(m->state);
		} else if (auto m = msg->as<sv::MessageUpdate>()) {
			for (auto &event : m->events) {
				c->serverState->applyEvent(event);
				c->clientState.applyEvent(event);
			}
		}
	}

	{
		sp::Canvas &canvas = c->testCanvas;
		canvas.clear();

		sf::Vec2 uiMouse = input.mousePosition * c->uiResolution;

		float cardHeight = 125.0f;
		float cardWidth = cardHeight * cl::Card::Aspect;
		float cardPad = 5.0f;
		float cardWidthPad = cardWidth + cardPad;
		sf::Vec2 cardOrigin = { 20.0f, 700.0f - cardHeight };
		sf::Vec2 pos = cardOrigin;

		// HACK: Take the first character
		cl::Character *character = nullptr;
		for (sf::Box<cl::Entity> &entity : c->clientState.entities) {
			if (!entity) continue;
			if (auto chr = entity->as<cl::Character>()) {
				character = chr;
			}
		}

		if (character) {
			sf::Vec2 relMouse = uiMouse - cardOrigin;
			if (character->cards.size > 0 && relMouse.x >= -5.0f && relMouse.x <= cardWidthPad * (float)character->cards.size + 5.0f && relMouse.y >= -5.0f && relMouse.y <= cardHeight + 5.0f) {
				c->hoveredCard = sf::clamp((int32_t)(relMouse.x / cardWidthPad), 0, (int32_t)character->cards.size - 1);
			} else {
				c->hoveredCard = -1;
			}

			int32_t ix = 0;
			for (cl::Card &card : character->cards) {
				while (ix >= c->cardGuiState.size) c->cardGuiState.push();
				CardGuiState &guiState = c->cardGuiState[ix];

				float hoverTarget = 0.0f;

				if (ix == c->selectedCard) {
					hoverTarget = 6.0f;
				} else if (ix == c->hoveredCard) {
					hoverTarget = 3.0f;
				}

				lerpExp(guiState.hover, hoverTarget, 40.0f, 5.0f, dt);

				sf::Vec2 cardPos = pos;
				cardPos.y -= guiState.hover;
				sf::Mat23 transform = sf::mat2D::translate(cardPos) * sf::mat2D::scale(cardWidth / 100.0f);
				canvas.pushTransform(transform);
				drawCard(c, canvas, card);
				canvas.popTransform();

				pos.x += cardWidthPad;

				ix++;
			}

			if (c->selectedOffset != 0.0f) {
				lerpExp(c->selectedOffset, 0.0f, 40.0f, 5.0f, dt);
			}

			if (c->selectedCard >= 0) {

				cl::Card &card = character->cards[c->selectedCard];
				sf::Mat23 bigTransform = sf::mat2D::translate(sf::Vec2(20.0f, 200.0f + c->selectedOffset)) * sf::mat2D::scale(230.0f / 100.0f);

				canvas.pushTransform(bigTransform);
				drawCard(c, canvas, card);
				canvas.popTransform();
			}

			if (c->hoveredCard >= 0 && c->hoveredCard != c->selectedCard) {
				cl::Card &card = character->cards[c->hoveredCard];
				sf::Mat23 bigTransform = sf::mat2D::translate(sf::Vec2(20.0f, 190.0f)) * sf::mat2D::scale(230.0f / 100.0f);

				canvas.pushTransform(bigTransform);
				drawCard(c, canvas, card);
				canvas.popTransform();
			}

		}

		for (sapp_event &e : input.events) {
			if (e.type == SAPP_EVENTTYPE_MOUSE_DOWN) {
				if (e.mouse_button == 0) {
					if (c->selectedCard != c->hoveredCard) {
						c->selectedCard = c->hoveredCard;
						c->selectedOffset = -10.0f;
					} else {
						c->selectedCard = -1;
					}
				}
			} else if (e.type == SAPP_EVENTTYPE_KEY_DOWN && !e.key_repeat) {
				if (e.key_code >= '1' && e.key_code <= '9' && character) {
					int32_t ix = (int32_t)(e.key_code - '1');
					if (ix == c->selectedCard) {
						c->selectedCard = -1;
					} else if (ix < character->cards.size) {
						c->selectedOffset = -10.0f;
						c->selectedCard = ix;
					}
				}
			} else if (e.type == SAPP_EVENTTYPE_TOUCHES_BEGAN || e.type == SAPP_EVENTTYPE_TOUCHES_MOVED) {
				for (sapp_touchpoint &touch : sf::slice(e.touches, e.num_touches)) {
					if (!touch.changed) continue;
					sf::Vec2 uiTouch = sf::Vec2(touch.pos_x, touch.pos_y) / sf::Vec2(c->resolution) * c->uiResolution;
					sf::Vec2 relTouch = uiTouch - cardOrigin;
					if (character && character->cards.size > 0 && relTouch.x >= -5.0f && relTouch.x <= cardWidthPad * (float)character->cards.size + 5.0f && relTouch.y >= -5.0f && relTouch.y <= cardHeight + 5.0f) {
						c->selectedCard = sf::clamp((int32_t)(relTouch.x / cardWidthPad), 0, (int32_t)character->cards.size - 1);
					}
				}
			}
		}

		canvas.prepareForRendering();
	}

	return false;
}

void clientFree(ClientMain *client)
{
	bqws_free_socket(client->ws);
	delete client;
}

sg_image clientRender(ClientMain *c)
{

	// HACK HACK
	{
		float t = (float)stm_sec(stm_now())*0.5f;
		uint32_t ix = 0;
		for (cl::PointLight &light : c->clientState.pointLights) {
			if (ix++ < 3) {
				light.position.x = sinf(t) * 5.0f;
				light.position.z = cosf(t) * 5.0f;
			}
			c->clientState.shadowCache.updatePointLight(c->clientState, light);
			t += sf::F_2PI / 3.0f;
		}
	}


	{
		sg_pass_action action = { };
		action.colors[0].action = SG_ACTION_CLEAR;
		action.colors[0].val[0] = 0.01f;
		action.colors[0].val[1] = 0.01f;
		action.colors[0].val[2] = 0.01f;
		action.colors[0].val[3] = 1.0f;
		action.depth.action = SG_ACTION_CLEAR;
		action.depth.val = 1.0f;

		sp::beginPass(c->mainPass, &action);

		sf::Vec3 cameraPosition = sf::Vec3(0.0f, 10.0f, 6.0f);
		sf::Mat44 view = sf::mat::look(cameraPosition, sf::Vec3(0.0f, -1.0f, -0.5f));
		sf::Mat44 proj = sf::mat::perspectiveD3D(1.0f, (float)c->resolution.x/(float)c->resolution.y, 0.1f, 20.0f);
		sf::Mat44 viewProj = proj * view;

		sf::Frustum frustum { viewProj, sg_query_features().origin_top_left ? 0.0f : -1.0f };
		for (auto &pair : c->clientState.chunks) {
			cl::MapChunkGeometry &chunkGeo = pair.val.geometry;
			if (!chunkGeo.main.vertexBuffer.buffer.id) continue;
			if (!frustum.intersects(chunkGeo.main.bounds)) continue;

			c->tempMeshPipe.bind();

#if 0

			TestMesh_Transform_t transform;
			viewProj.writeColMajor44(transform.transform);
			sf::Mat44().writeColMajor44(transform.normalTransform);
			sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_TestMesh_Transform, &transform, sizeof(transform));

			TestMesh_Pixel_t pu = { };
			memcpy(pu.cameraPosition, cameraPosition.v, sizeof(cameraPosition));
			pu.numLightsF = (float)c->clientState.pointLights.size;
			float (*dst)[4] = pu.lightData;
			for (cl::PointLight &light : c->clientState.pointLights) {
				dst[0][0] = light.position.x;
				dst[0][1] = light.position.y;
				dst[0][2] = light.position.z;
				dst[0][3] = light.radius;
				dst[1][0] = light.color.x;
				dst[1][1] = light.color.y;
				dst[1][2] = light.color.z;
				dst[1][3] = 0.0f;
				dst[2][0] = light.shadowMul.x;
				dst[2][1] = light.shadowMul.y;
				dst[2][2] = light.shadowMul.z;
				dst[2][3] = 0.0f;
				dst[3][0] = light.shadowBias.x;
				dst[3][1] = light.shadowBias.y;
				dst[3][2] = light.shadowBias.z;
				dst[3][3] = 0.0f;
				dst += 4;
			}

			sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_TestMesh_Pixel, &pu, sizeof(pu));
			sg_bindings bindings = { };
			bindings.vertex_buffers[0] = chunkGeo.main.vertexBuffer.buffer;
			bindings.index_buffer = chunkGeo.main.indexBuffer.buffer;
			bindings.fs_images[SLOT_TestMesh_shadowGrid] = c->clientState.shadowCache.shadowCache.image;
			bindings.fs_images[SLOT_TestMesh_albedoAtlas] = cl::TileMaterial::getAtlasImage(cl::MaterialTexture::Albedo);
			bindings.fs_images[SLOT_TestMesh_normalAtlas] = cl::TileMaterial::getAtlasImage(cl::MaterialTexture::Normal);
			bindings.fs_images[SLOT_TestMesh_maskAtlas] = cl::TileMaterial::getAtlasImage(cl::MaterialTexture::Mask);
			sg_apply_bindings(&bindings);

#endif

			UBO_Transform tu = { };
			tu.transform = viewProj;
			tu.normalTransform = sf::Mat44();

			UBO_Pixel pu = { };
			pu.numLightsF = (float)c->clientState.pointLights.size;
			pu.cameraPosition = cameraPosition;
			sf::Vec4 *dst = pu.pointLightData;
			for (cl::PointLight &light : c->clientState.pointLights) {
				dst[0].x = light.position.x;
				dst[0].y = light.position.y;
				dst[0].z = light.position.z;
				dst[0].w = light.radius;
				dst[1].x = light.color.x;
				dst[1].y = light.color.y;
				dst[1].z = light.color.z;
				dst[1].w = 0.0f;
				dst[2].x = light.shadowMul.x;
				dst[2].y = light.shadowMul.y;
				dst[2].z = light.shadowMul.z;
				dst[2].w = 0.0f;
				dst[3].x = light.shadowBias.x;
				dst[3].y = light.shadowBias.y;
				dst[3].z = light.shadowBias.z;
				dst[3].w = 0.0f;
				dst += 4;
			}

			bindUniformVS(c->testMeshShader, tu);
			bindUniformFS(c->testMeshShader, pu);

			sg_bindings bindings = { };
			bindings.vertex_buffers[0] = chunkGeo.main.vertexBuffer.buffer;
			bindings.index_buffer = chunkGeo.main.indexBuffer.buffer;

			bindImageFS(c->testMeshShader, bindings, CL_SHADOWCACHE_TEX, c->clientState.shadowCache.shadowCache.image);

			bindImageFS(c->testMeshShader, bindings, TEX_albedoAtlas, cl::TileMaterial::getAtlasImage(cl::MaterialTexture::Albedo));
			bindImageFS(c->testMeshShader, bindings, TEX_normalAtlas, cl::TileMaterial::getAtlasImage(cl::MaterialTexture::Normal));
			bindImageFS(c->testMeshShader, bindings, TEX_maskAtlas, cl::TileMaterial::getAtlasImage(cl::MaterialTexture::Mask));

			sg_apply_bindings(&bindings);

			sg_draw(0, chunkGeo.main.numInidces, 1);
		}

		for (cl::Entity *entity : c->clientState.entities) {
			if (!entity) continue;

			if (cl::Character *chr = entity->as<cl::Character>()) {
				c->tempSkinnedMeshPipe.bind();

				if (!chr->model.isLoaded()) continue;
				cl::ModelInfo &modelInfo = chr->model->data;
				if (!modelInfo.modelRef.isLoaded()) continue;
				sp::Model *model = modelInfo.modelRef;
				cl::AnimationInfo &animation = modelInfo.animations[0];
				if (!animation.animationRef.isLoaded()) continue;
				sp::Animation *anim = animation.animationRef;

				sf::SmallArray<uint32_t, 64> boneMapping;
				boneMapping.resizeUninit(anim->bones.size);
				anim->generateBoneMapping(model, boneMapping);

				sf::SmallArray<sp::BoneTransform, sp::MaxBones> boneTransforms;
				sf::SmallArray<sf::Mat34, sp::MaxBones> boneWorld;
				boneTransforms.resizeUninit(model->bones.size);
				boneWorld.resizeUninit(model->bones.size);

				sf::Vec3 worldPos = sf::Vec3(chr->position.x, 0.0f, chr->position.y);
				sf::Mat34 world = sf::mat::translate(worldPos) * sf::mat::scale(modelInfo.scale);

				float animTime = fmodf((float)stm_sec(stm_now()), anim->duration);

				for (uint32_t i = 0; i < model->bones.size; i++) {
					boneTransforms[i] = model->bones[i].bindTransform;
				}

				anim->evaluate(animTime, boneMapping, boneTransforms);

				sp::boneTransformToWorld(model, boneWorld, boneTransforms, world);

				UBO_Pixel pu = { };
				pu.numLightsF = (float)c->clientState.pointLights.size;
				pu.cameraPosition = cameraPosition;
				sf::Vec4 *dst = pu.pointLightData;
				for (cl::PointLight &light : c->clientState.pointLights) {
					dst[0].x = light.position.x;
					dst[0].y = light.position.y;
					dst[0].z = light.position.z;
					dst[0].w = light.radius;
					dst[1].x = light.color.x;
					dst[1].y = light.color.y;
					dst[1].z = light.color.z;
					dst[1].w = 0.0f;
					dst[2].x = light.shadowMul.x;
					dst[2].y = light.shadowMul.y;
					dst[2].z = light.shadowMul.z;
					dst[2].w = 0.0f;
					dst[3].x = light.shadowBias.x;
					dst[3].y = light.shadowBias.y;
					dst[3].z = light.shadowBias.z;
					dst[3].w = 0.0f;
					dst += 4;
				}

				UBO_SkinTransform su = { };
				su.worldToClip = viewProj;

				bindUniformVS(c->testSkinShader, su);
				bindUniformFS(c->testSkinShader, pu);

				// TestSkin_FragUniform_t fragUniform = { };

				// sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_TestSkin_FragUniform, &fragUniform, sizeof(fragUniform));

				for (sp::Mesh &mesh : model->meshes) {
					UBO_Bones bones;
					for (uint32_t i = 0; i < mesh.bones.size; i++) {
						sp::MeshBone &meshBone = mesh.bones[i];
						sf::Mat34 transform = boneWorld[meshBone.boneIndex] * meshBone.meshToBone;
						memcpy(bones.bones[i * 3 + 0].v, transform.getRow(0).v, sizeof(sf::Vec4));
						memcpy(bones.bones[i * 3 + 1].v, transform.getRow(1).v, sizeof(sf::Vec4));
						memcpy(bones.bones[i * 3 + 2].v, transform.getRow(2).v, sizeof(sf::Vec4));
					}

					bindUniformVS(c->testSkinShader, bones);

					sg_bindings binds = { };
					binds.vertex_buffers[0] = model->vertexBuffer.buffer;
					binds.index_buffer = model->indexBuffer.buffer;
					binds.index_buffer_offset = mesh.indexBufferOffset;
					binds.vertex_buffer_offsets[0] = mesh.streams[0].offset;
					bindImageFS(c->testMeshShader, binds, CL_SHADOWCACHE_TEX, c->clientState.shadowCache.shadowCache.image);
					sg_apply_bindings(&binds);

					sg_draw(0, mesh.numIndices, 1);
				}
			}
		}

		sp::endPass();
	}

#if 0
	{
		sp::beginPass(c->tonemapPass, nullptr);

		{
			c->tonemapPipe.bind();

			sg_bindings bindings = { };
			bindings.fs_images[SLOT_Postprocess_mainImage] = c->mainTarget.image;
			bindings.vertex_buffers[0] = gameShaders.fullscreenTriangleBuffer;
			sg_apply_bindings(&bindings);

			sg_draw(0, 3, 1);
		}

		sp::endPass();
	}

	{
		sp::beginPass(c->fxaaPass, nullptr);

		{
			c->fxaaPipe.bind();

			Fxaa_Pixel_t pixel;
			pixel.rcpTexSize = sf::Vec2(1.0f) / sf::Vec2(c->tonemapPass.resolution);
			sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_Fxaa_Pixel, &pixel, sizeof(pixel));

			sg_bindings bindings = { };
			bindings.fs_images[SLOT_Fxaa_Pixel] = c->tonemapTarget.image;
			bindings.vertex_buffers[0] = gameShaders.fullscreenTriangleBuffer;
			sg_apply_bindings(&bindings);

			sg_draw(0, 3, 1);
		}

		sp::endPass();
	}
#endif

	return c->mainTarget.image;
}

void clientRenderGui(ClientMain *c)
{
	sp::CanvasRenderOpts opts = sp::CanvasRenderOpts::pixels(c->uiResolution);
	c->testCanvas.render(opts);
}

void clientDoMoveTemp(ClientMain *c)
{
	auto move = sf::box<sv::ActionMove>();
	move->entity = 1;
	move->position = sf::Vec2i(5, 5);

	sv::MessageAction action;
	action.action = move;
	writeMessage(c->ws, &action, c->name, serverName);
}
