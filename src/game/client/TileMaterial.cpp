#include "TileMaterial.h"

#include "sp/ContentFile.h"
#include "sp/Renderer.h"
#include "sf/Mutex.h"

#include "ext/sokol/sokol_gfx.h"
#include "ext/sp_tools_common.h"

namespace cl {

static const char *getPixelFormatSuffix(sg_pixel_format format)
{
    switch (format) {
    case SG_PIXELFORMAT_RGBA8: return "rgba8";
    case SG_PIXELFORMAT_BQQ_SRGBA8: return "rgba8";
    case SG_PIXELFORMAT_BC1_RGBA: return "bc1";
    case SG_PIXELFORMAT_BQQ_BC1_SRGB: return "bc1";
    case SG_PIXELFORMAT_BC3_RGBA: return "bc3";
    case SG_PIXELFORMAT_BQQ_BC3_SRGB: return "bc3";
    case SG_PIXELFORMAT_BQQ_ASTC_4X4_RGBA: return "astc4x4";
    case SG_PIXELFORMAT_BQQ_ASTC_4X4_SRGB: return "astc4x4";
    case SG_PIXELFORMAT_BQQ_ASTC_8X8_RGBA: return "astc8x8";
    case SG_PIXELFORMAT_BQQ_ASTC_8X8_SRGB: return "astc8x8";
    default: return "";
    }
}

struct TileMaterialAtlasTexture
{
	sp::Texture texture;
	uint32_t textureExtent;
	uint32_t resolutionX, resolutionY;
	uint32_t numMips = 0;
	uint32_t numAllocatedMips = 0;
	sg_pixel_format pixelFormat = SG_PIXELFORMAT_NONE;

	void init(uint32_t numSlotsX, uint32_t numSlotsY, uint32_t extent, sg_pixel_format format, const char *label)
	{
		numMips = 0;
		numAllocatedMips = 0;

		for (uint32_t e = extent; e >= 8; e /= 2) {
			numMips++;
		}

		pixelFormat = format;
		textureExtent = extent;
		resolutionX = numSlotsX * extent;
		resolutionY = numSlotsY * extent;

		for (uint32_t e = sf::max(resolutionX, resolutionY); e >= 1; e /= 2) {
			numAllocatedMips++;
		}

		sg_image_desc desc = { };
		desc.label = label;
		desc.pixel_format = format;
		desc.width = (int)resolutionX;
		desc.height = (int)resolutionY;
		desc.num_mipmaps = (int)numAllocatedMips;
		desc.bqq_copy_target = true;
		desc.mag_filter = SG_FILTER_LINEAR;
		desc.min_filter = SG_FILTER_LINEAR_MIPMAP_LINEAR;
		desc.max_lod = (float)(numMips - 1);
		// TODO: Config
		desc.max_anisotropy = 4;
		texture.init(desc);
	}
};

struct TileMaterialContext
{
	sf::Mutex mutex;

	uint32_t numSlotsX = 16, numSlotsY = 16;

	TileMaterialAtlasTexture atlases[(uint32_t)MaterialTexture::Count];

	sf::Array<uint32_t> freeSlots;
	uint32_t nextFreeSlot = 0;

	uint32_t allocSlot() {
		if (freeSlots.size > 0) {
			return freeSlots.popValue();
		} else {
			uint32_t res = nextFreeSlot++;
			sf_assert(res < numSlotsX * numSlotsY);
			return res;
		}
	}

	void freeSlot(uint32_t slot) {
		freeSlots.push(slot);
	}
};

TileMaterialContext g_tileMaterialContext;

struct TileMaterialImp : TileMaterial
{
	virtual void assetStartLoading() final;
	virtual void assetUnload() final;

	uint32_t slot = ~0u;
	uint32_t textureLoadedMask = 0;
};

sp::AssetType TileMaterial::SelfType = { "TileMaterial", sizeof(TileMaterialImp), sizeof(TileMaterial::PropType),
	[](Asset *a) { new ((TileMaterialImp*)a) TileMaterialImp(); }
};

static void loadTextureImp(void *user, const sp::ContentFile &file, MaterialTexture texture)
{
	TileMaterialImp *imp = (TileMaterialImp*)user;

	uint32_t textureBit = 1 << (uint32_t)texture;
	sf_assert((imp->textureLoadedMask & textureBit) == 0);
	imp->textureLoadedMask |= textureBit;
	bool allLoaded = imp->textureLoadedMask == (1 << (uint32_t)MaterialTexture::Count) - 1;

	if (file.size > 0) {
		TileMaterialContext &ctx = g_tileMaterialContext;
		TileMaterialAtlasTexture &atlas = ctx.atlases[(uint32_t)texture];

		sptex_util su;
		sptex_util_init(&su, file.data, file.size);

		sg_image_desc d = { };
		d.pixel_format = atlas.pixelFormat;
		d.num_mipmaps = atlas.numMips;
		d.width = atlas.textureExtent;
		d.height = atlas.textureExtent;

		sptex_header header = sptex_decode_header(&su);
		uint32_t extent = header.info.width;
		sf_assert(extent == header.info.height);
		sf_assert(extent >= atlas.textureExtent);

		uint32_t mipDrop = 0;
		while (extent > atlas.textureExtent) {
			extent /= 2;
			mipDrop++;
		}
		sf_assert(extent == atlas.textureExtent);

		for (uint32_t mipI = 0; mipI < atlas.numMips; mipI++) {
			d.content.subimage[0][mipI].ptr = sptex_decode_mip(&su, mipDrop + mipI);
			d.content.subimage[0][mipI].size = header.s_mips[mipDrop + mipI].uncompressed_size;
		}

		uint32_t offsetX = (imp->slot % ctx.numSlotsX) * atlas.textureExtent;
		uint32_t offsetY = (imp->slot / ctx.numSlotsY) * atlas.textureExtent;

		if (!spfile_util_failed(&su.file)) {
			sg_bqq_update_subimage(atlas.texture.image, &d, (int)offsetX, (int)offsetY);
		}

		spfile_util_free(&su.file);
	}

	if (allLoaded) {
		imp->assetFinishLoading();
	}
}

static void loadAlbedoImp(void *user, const sp::ContentFile &file) { loadTextureImp(user, file, MaterialTexture::Albedo); }
static void loadNormalImp(void *user, const sp::ContentFile &file) { loadTextureImp(user, file, MaterialTexture::Normal); }
static void loadMaskImp(void *user, const sp::ContentFile &file) { loadTextureImp(user, file, MaterialTexture::Mask); }

void TileMaterialImp::assetStartLoading()
{
    TileMaterialContext &ctx = g_tileMaterialContext;
	sf::SmallStringBuf<256> path;

	{
		sf::MutexGuard mg(ctx.mutex);
		slot = ctx.allocSlot();
	}

    sf::Vec2 rcpSize = sf::Vec2(1.0f) / sf::Vec2((float)ctx.numSlotsX, (float)ctx.numSlotsY);
    sf::Vec2i offset = { (int32_t)(slot % ctx.numSlotsX), (int32_t)(slot / ctx.numSlotsX) };
    uvBase = sf::Vec2(offset) * rcpSize;
    uvScale = rcpSize;

	// TODO: Formats

    path.clear(); path.format("%s_albedo.%s.sptex", name.data, getPixelFormatSuffix(ctx.atlases[(uint32_t)MaterialTexture::Albedo].pixelFormat));
	sp::ContentFile::loadMainThread(path, &loadAlbedoImp, this);

    path.clear(); path.format("%s_normal.%s.sptex", name.data, getPixelFormatSuffix(ctx.atlases[(uint32_t)MaterialTexture::Normal].pixelFormat));
	sp::ContentFile::loadMainThread(path, &loadNormalImp, this);

    path.clear(); path.format("%s_mask.%s.sptex", name.data, getPixelFormatSuffix(ctx.atlases[(uint32_t)MaterialTexture::Mask].pixelFormat));
	sp::ContentFile::loadMainThread(path, &loadMaskImp, this);
}

void TileMaterialImp::assetUnload()
{
	TileMaterialContext &ctx = g_tileMaterialContext;
	sf::MutexGuard mg(ctx.mutex);

	if (slot != ~0u) {
		ctx.freeSlot(slot);
		slot = ~0u;
	}
}

sg_image TileMaterial::getAtlasImage(MaterialTexture texture)
{
	TileMaterialContext &ctx = g_tileMaterialContext;
	return ctx.atlases[(uint32_t)texture].texture.image;
}

void TileMaterial::globalInit()
{
	TileMaterialContext &ctx = g_tileMaterialContext;
    
    sg_pixel_format albedoFormats[] = {
        SG_PIXELFORMAT_BQQ_BC1_SRGB,
        SG_PIXELFORMAT_BQQ_ASTC_4X4_SRGB,
        SG_PIXELFORMAT_BQQ_SRGBA8,
    };
    
    sg_pixel_format normalFormats[] = {
        SG_PIXELFORMAT_BC5_RG,
        SG_PIXELFORMAT_BQQ_ASTC_4X4_RGBA,
        SG_PIXELFORMAT_BC3_RGBA,
        SG_PIXELFORMAT_RGBA8,
    };
    
    sg_pixel_format maskFormats[] = {
        SG_PIXELFORMAT_BC3_RGBA,
        SG_PIXELFORMAT_BQQ_ASTC_8X8_SRGB,
        SG_PIXELFORMAT_RGBA8,
    };

    for (sg_pixel_format format : albedoFormats) {
        if (!sg_query_pixelformat(format).sample) continue;
        ctx.atlases[(uint32_t)MaterialTexture::Albedo].init(ctx.numSlotsX, ctx.numSlotsY, 256, format, "TileMaterial albedo");
        break;
    }

    for (sg_pixel_format format : normalFormats) {
        if (!sg_query_pixelformat(format).sample) continue;
    	ctx.atlases[(uint32_t)MaterialTexture::Normal].init(ctx.numSlotsX, ctx.numSlotsY, 256, format, "TileMaterial normal");
        break;
    }
    
    for (sg_pixel_format format : maskFormats) {
        if (!sg_query_pixelformat(format).sample) continue;
    	ctx.atlases[(uint32_t)MaterialTexture::Mask].init(ctx.numSlotsX, ctx.numSlotsY, 256, format, "TileMaterial mask");
        break;
    }
}

void TileMaterial::globalCleanup()
{
	g_tileMaterialContext.~TileMaterialContext();
	new (&g_tileMaterialContext) TileMaterialContext();
}

}

