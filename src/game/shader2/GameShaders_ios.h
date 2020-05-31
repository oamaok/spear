#pragma once

#include "sf/Vector.h"
#include "sf/Matrix.h"

#define SpShader_TestMesh 0
#define SpShader_TestSkin 1
#define SpShaderDataSize 58721

#define SP_SHADOWGRID_USE_ARRAY 0
#define SP_NORMALMAP_REMAP 1
#define SP_NUM_PERMUTATIONS 2

struct UBO_Transform {
	static const constexpr uint32_t UboIndex = 1;

	sf::Mat44 transform;
	sf::Mat44 normalTransform;
};

struct UBO_Pixel {
	static const constexpr uint32_t UboIndex = 2;

	float numLightsF;
	uint32_t _sp_pad0[3];
	sf::Vec3 cameraPosition;
	uint32_t _sp_pad1[1];
	sf::Vec4 pointLightData[64];
};

struct UBO_SkinTransform {
	static const constexpr uint32_t UboIndex = 3;

	sf::Mat44 worldToClip;
};

struct UBO_Bones {
	static const constexpr uint32_t UboIndex = 4;

	sf::Vec4 bones[192];
};

#define TEX_shadowGrid3D 1
#define TEX_albedoAtlas 2
#define TEX_normalAtlas 3
#define TEX_maskAtlas 4
#define TEX_shadowGridArray 5

struct SpShaderInfo;
struct SpPermutationInfo;
struct SpUniformBlockInfo;
struct SpSamplerInfo;
struct SpAttribInfo;
extern const SpShaderInfo spShaders[2];
extern const SpPermutationInfo spPermutations[10];
extern const SpUniformBlockInfo spUniformBlock[5];
extern const SpSamplerInfo spSamplers[6];
extern const SpAttribInfo spAttribs[7];
extern const char spShaderData[2920];