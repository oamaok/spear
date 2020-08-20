#include "GameShaders_ios.h"
#include "game/ShadersDesc.h"

const SpShaderInfo spShaders[] = {
	{
		"TestMesh",
		{ {
			{  },
			0, 1,
		}, {
			{ {0,2}, {1,2} },
			1, 4,
		} }
	},
	{
		"TestSkin",
		{ {
			{  },
			5, 1,
		}, {
			{ {0,2}, {1,2} },
			6, 4,
		} }
	},
};

const SpPermutationInfo spPermutations[] = {
	{ { 1 }, {  }, { 1,2,3,4,5 }, 0, 1910 },
	{ { 2 }, { 1,2,3,4 }, {  }, 1910, 7429 },
	{ { 2 }, { 1,2,3,4 }, {  }, 9339, 7429 },
	{ { 2 }, { 5,2,3,4 }, {  }, 16768, 8103 },
	{ { 2 }, { 5,2,3,4 }, {  }, 24871, 8103 },
	{ { 3,4 }, {  }, { 1,6,7,8,9,10 }, 32974, 2089 },
	{ { 2 }, { 1,6,7,8 }, {  }, 35063, 7377 },
	{ { 2 }, { 1,6,7,8 }, {  }, 42440, 7377 },
	{ { 2 }, { 5,6,7,8 }, {  }, 49817, 8051 },
	{ { 2 }, { 5,6,7,8 }, {  }, 57868, 8051 },
};

const SpUniformBlockInfo spUniformBlocks[] = {
	{ }, // Null uniform block
	{ "Transform", 64 },
	{ "Pixel", 1056 },
	{ "SkinTransform", 64 },
	{ "Bones", 3072 },
};
const SpSamplerInfo spSamplers[] = {
	{ }, // Null sampler
	{ "shadowGrid3D", (uint32_t)SG_IMAGETYPE_3D },
	{ "albedoAtlas", (uint32_t)SG_IMAGETYPE_2D },
	{ "normalAtlas", (uint32_t)SG_IMAGETYPE_2D },
	{ "maskAtlas", (uint32_t)SG_IMAGETYPE_2D },
	{ "shadowGridArray", (uint32_t)SG_IMAGETYPE_ARRAY },
	{ "albedoTexture", (uint32_t)SG_IMAGETYPE_2D },
	{ "normalTexture", (uint32_t)SG_IMAGETYPE_2D },
	{ "maskTexture", (uint32_t)SG_IMAGETYPE_2D },
};
const SpAttribInfo spAttribs[] = {
	{ }, // Null attrib
	{ "a_position", 0 },
	{ "a_normal", 1 },
	{ "a_tangent", 2 },
	{ "a_uv", 3 },
	{ "a_tint", 4 },
	{ "a_uv", 1 },
	{ "a_normal", 2 },
	{ "a_tangent", 3 },
	{ "a_indices", 4 },
	{ "a_weights", 5 },
};

const char spShaderData[] = 
	"\x28\xb5\x2f\xfd\xa0\x7f\x01\x01\x00\xcd\x69\x00\xca\x72\x0c\x15\x2b\xc0\xcc\x86\x06\xa8\x02\x75\xc8\x99\x29\x9b\xda\x57\x81\x76"
	"\xe7\xd7\x44\xc1\x69\x1d\x7a\x93\xb3\x62\x31\x53\x09\x6c\x2d\x0f\x03\x2c\xf4\x2d\x03\x00\x00\x00\x41\x04\x2b\x11\x43\x01\x4d\x01"
	"\x42\x01\xd4\x52\xea\x9c\x6c\xb3\xc2\x69\xed\x7c\xd0\x3d\xc9\xf0\x8b\xa4\x75\xda\x67\xe9\x9b\x30\xd6\x17\xe9\xa3\xb3\xd2\x68\x9d"
	"\xf6\xcc\x5b\x8c\x55\xf6\x84\x29\x09\x4f\xb0\x9e\x79\xf9\x5c\xa5\x4d\xca\xe8\xac\x8d\xb5\xc9\x26\x5b\x85\xd6\x42\x4a\x35\xaf\x3a"
	"\xe6\x8b\xa4\x30\xc2\xd7\x22\x85\x10\xc2\x57\x1f\x7d\xd6\x5a\x1b\x63\x8b\x0f\x81\xb7\xd8\x47\xf2\xd2\x1c\xf7\x4d\x73\xda\x37\x16"
	"\x9b\x26\x57\x82\x0c\x18\xd0\xa9\x1b\xf9\xe7\xad\xc5\xfa\x85\xe8\x62\xae\xd1\x19\xbf\x58\x67\x89\x6d\x91\x21\xe1\xd4\x2b\x2c\x0b"
	"\x44\x5d\xb7\x22\x5d\xc6\x20\x08\x82\x1e\xeb\x2c\xe9\xbb\xc4\x0c\xef\xbf\x77\x3f\xba\xcb\x70\xaf\x32\x32\xef\xfd\x5e\x9d\x31\x30"
	"\x28\x20\x49\xf2\x51\xe6\x18\x29\x15\x44\xf9\x07\x73\xc6\xeb\x1a\x4b\xf4\xed\x4e\xba\x13\xbe\xb5\x03\x03\x63\x69\x1a\xb2\x25\xc6"
	"\x31\x55\x5d\x5c\xbc\xde\xb9\x67\x6a\x4f\x10\xe5\x61\xaf\x36\xd4\x74\xbe\x93\x09\x0a\x8e\x29\x4b\x08\xb6\x45\x24\xf1\x5d\xf9\x68"
	"\xc5\x7e\xb7\x14\xf9\xff\x57\x90\xa0\x30\xe1\x3b\x42\xe9\xcb\x2d\xbe\x43\x2a\xde\x29\x05\x05\xc7\x63\xc8\x9d\x28\x3c\xb9\xfa\x2b"
	"\x05\x85\x02\x1a\x8e\x43\x34\xc9\x54\x81\x8c\x2c\x45\x9d\x82\xa4\xdb\xf1\x84\xc0\xe3\xa8\x92\x20\x3a\x0b\x50\x50\x08\x40\xe1\xe9"
	"\xea\xbb\x6f\x3e\x35\x48\x74\xa6\xa8\x3c\x69\xd0\x10\xe0\x23\xcc\xb7\x96\xe4\x87\xe4\x4e\x6d\x2d\xb9\x39\x7a\x89\xbe\x12\xe4\xef"
	"\x44\x72\xf9\x24\x20\xad\xc2\x16\x21\xa4\xd5\x36\x58\x1d\x74\x1a\x04\x7b\x78\x3c\xf3\x10\x4c\x46\xf8\x90\x11\xf1\x01\xbf\x79\x1e"
	"\xa2\x45\xbe\xf9\x26\x12\xc9\x11\xe4\x17\x20\xbf\x52\x05\xc1\x13\x05\x41\x3c\x30\x38\xd0\x9c\xe6\xf0\xcf\x17\x70\xd0\x2d\xcc\xd7"
	"\x38\xa3\x93\xaf\xbd\x1a\x65\x75\xb0\x3e\x67\x4e\xb6\xc6\x11\xc3\xf3\x0c\xdf\xf0\x11\x11\xff\xfc\x83\x84\xc4\x43\xc2\x92\x48\x98"
	"\x67\x8c\x75\x1e\x81\x7a\xe6\x9f\x7f\x58\x12\xc9\xbb\x1a\x93\x74\x23\x1e\xdf\x48\xf4\x1d\x4d\x30\x07\x09\x12\x40\x0f\x8f\x77\x2c"
	"\x4b\xb4\x58\x8b\x7c\xc3\x33\xf6\x0c\xdf\xde\x8b\x56\xc3\x37\xc7\x45\xf4\x1d\x3d\x7e\xe1\x9b\x16\x81\x00\xc9\x92\x06\xa4\x02\x82"
	"\x78\xb8\x34\x76\x71\xf1\x7c\x4b\x1b\x02\xed\x8f\x18\x71\x5e\xdc\x7e\x1f\xba\xce\x7b\x82\x14\x97\xe3\xc9\x85\x40\x11\x86\x00\xb9"
	"\xd3\x73\x2d\xba\x13\x04\x4d\x14\x04\xd5\x35\x24\x59\x62\x52\x69\x09\x5b\x99\x02\x40\xaf\xf6\xf3\x15\xa7\xe2\x34\x1b\x97\xd3\x58"
	"\x4e\x9e\xce\x41\x99\x52\x51\xaa\x0a\xf3\x4b\xc3\x28\x6a\x73\xc8\x3f\x1b\x97\xe5\x30\xe6\x9d\xc7\xd1\xf9\x1f\xa7\x57\x7b\x82\x84"
	"\x7f\x38\x8e\x44\xaa\x8c\x8b\x8b\x9f\x14\xfa\x05\x7d\x1b\x0c\x0c\x9d\x21\x2c\xb9\x9d\xca\xcf\x2c\xdc\x39\xa6\x38\x16\xda\xd0\x08"
	"\xb1\x47\x37\x9e\x83\x9e\xbc\x1c\xe4\x58\x9e\xa7\x11\x72\x58\x0e\xe5\x9f\x9f\x5c\x7e\xa7\x5f\x28\xbf\xd0\x29\xbd\x21\x70\x82\xc2"
	"\x04\xf9\x66\x8b\xd3\xc1\x48\x27\x94\xb6\x45\xe7\x7d\x23\xdf\x94\x4e\xd6\xb3\x56\x58\x6d\x7b\xd0\xda\x58\xed\x6b\xac\x68\x51\x9e"
	"\xe1\x1d\x9e\xf2\x30\x4f\x79\xff\x3c\xe6\x15\xb5\x27\x0b\x4f\x77\x82\xf5\xee\x1f\x6d\xd3\x2a\xb4\x15\xc6\x0a\x2b\x8c\x0d\x3e\x37"
	"\x29\x9d\xd4\xba\x46\xad\x6d\xcd\xc5\xf6\xac\x94\x0f\xd2\x29\xa7\x8b\x8c\x95\xcf\x59\x1a\x25\x8d\x12\xca\x68\xa1\x7d\x93\x3e\xeb"
	"\x3c\x57\xa3\x7b\xed\x3d\x38\xe1\x84\xf6\xc9\xf7\xe4\xab\x0d\x3a\xd8\x5a\x86\x84\x57\x6c\x8b\x45\xbe\xe8\x66\x6c\xce\x3e\x17\x5f"
	"\xad\xd5\xc6\xd6\x9c\xb6\x5c\xce\x04\xa3\x32\x2e\xf3\x8c\x31\x86\x65\x7e\xb3\x30\x53\x4a\x40\x07\x45\x44\x3c\x1e\x12\xdf\x4c\x2c"
	"\x11\xdf\x3c\x1e\xcf\xf0\x12\x0c\x6f\xe4\x7f\x3e\xe2\xa1\xc3\x1b\xe0\xd7\xb1\x2c\x0c\x3f\x69\xac\xd0\x3a\x37\x9d\x6b\x36\xba\xe6"
	"\x9c\xc6\xde\xe1\x56\x1a\xf7\x7e\x7f\x82\xe5\x2c\x12\xa6\x60\xe9\x9b\xe5\x1b\x86\xbf\x36\x2c\x87\x7d\xc3\x1c\x07\x13\xf1\xbd\x11"
	"\x58\xd8\xc2\x72\xe0\xcd\x07\x9e\x54\x29\x70\x08\x08\x30\xcf\x47\xa8\xe0\x8d\x41\xa9\xa2\x30\x5f\x1c\x9e\x97\x58\x1a\xd6\x77\x84"
	"\x8a\x4e\x65\x8a\x6f\x78\xfe\x76\x09\x47\x15\xaa\xca\xd1\x65\x90\x25\x99\x2f\x90\x1e\x42\xf8\x9e\x73\x0e\x0c\xe7\xf9\x76\xfb\x8d"
	"\x3e\x72\x69\x9a\x54\x70\x2c\x4d\x93\x4b\x3d\xc0\x28\xf3\x04\x95\x1d\x5d\x0d\xea\x0b\x10\x88\x20\xe6\x55\xb7\x70\x9f\xd4\x41\x94"
	"\x5f\xc0\xd4\xcd\xf8\x85\xae\xf3\x63\x82\x82\x02\xf7\x5b\x49\x97\x82\xad\xac\x01\x95\x6b\xda\x69\x2f\xab\x72\x41\x09\xa5\x45\xd7"
	"\x53\x7d\x6f\x7a\x12\xdd\xbb\x44\xf7\x37\xe1\x5a\xb7\x09\xd7\xcf\x7d\xb3\x34\xd4\x9e\x2a\x95\x8d\x8d\xf6\x9e\x73\xcb\x75\xbd\xe7"
	"\x9c\xe5\x30\x6f\x39\x56\x76\x1c\x84\x2a\x9b\x2f\x90\xf9\xc5\xca\x0e\xfa\x85\xfa\x1e\xf4\x0b\xb8\x42\x61\x48\x8e\x74\x86\xcf\xe9"
	"\x0c\xa1\xe2\x59\x77\x91\xf1\x91\xc2\x7a\x1f\xa8\x5c\xb2\x4d\xd9\x66\x73\x59\x0e\x3e\x08\x9b\xf8\x22\x84\xb1\xbe\x18\x5f\xa4\x53"
	"\x46\xf8\x1c\xe4\xe0\x73\xf0\xd1\x4a\x2b\xad\xb4\xf6\x6f\x1c\xf5\x4d\xa3\x66\x73\xce\xcd\xe6\x36\x58\xa4\xab\xcd\xab\x6f\x2e\xcb"
	"\x69\x9e\x6f\x94\xf4\xcd\x86\xe5\x4c\xd2\x42\x0f\xa5\x6f\xf8\x4b\x6a\x12\xd3\x79\xe2\x24\x2a\x31\xbe\xb7\x14\x69\x52\x75\xa6\x7f"
	"\xfe\x92\xde\xc0\x75\x39\xbf\x50\xf4\xa6\x8a\xca\xf5\x4c\x09\x41\x17\xd3\xe8\x1c\xe1\x63\xc2\x41\x11\xff\x61\x20\xe0\xe8\x7c\x6f"
	"\x28\x38\x98\x67\xbe\xf2\x7c\xb3\x48\x59\x27\x7d\x33\xda\x07\xed\x63\x56\x6e\x11\xcb\x79\x1e\x0f\x96\x4b\x67\xcb\x61\x74\x66\x6c"
	"\xe2\xe1\xf4\x3c\xc5\xf3\x10\xa8\xab\x20\x32\xbf\xf1\xbc\x01\xb8\x34\xcf\x3f\x60\xef\x99\xbf\x2c\x67\x69\xd8\x44\xbf\x93\x7f\x91"
	"\x75\x9d\x35\x86\xcf\x35\x06\x21\x50\x77\xf2\x1c\x75\xa5\x6e\x60\x47\x38\x43\xba\xef\xe2\xe2\x9b\xae\x71\xc4\x42\x49\x69\x8c\x33"
	"\xb6\x69\x65\x6b\x2d\x3a\xf7\xa0\x73\x73\x52\x6a\xe1\xa3\x55\x4a\x5a\x1d\x0d\x83\x5d\xa8\x52\xe3\x98\x2a\x44\x33\x02\x00\x00\x00"
	"\x01\x08\x00\xb2\x09\x10\x08\xc3\x69\x98\x44\xad\xc7\xda\x12\x40\xa1\x50\x92\xc3\x40\x8a\x23\x39\x48\x19\x85\x0c\x31\x06\x88\x00"
	"\x00\x48\x00\x20\x00\x00\x18\x00\x10\x23\x00\xdc\xe0\x4e\xa9\xde\x7e\x65\x6f\xf7\xad\xef\xaf\xdb\x4b\x2b\xa4\x55\xb3\x7d\x6b\xa8"
	"\xc1\x2b\x7c\xb8\x1e\xdc\x0b\x07\xb0\x42\xb2\x1b\xb4\xbd\x73\x78\x63\x52\xe1\x6d\x00\x5b\xee\x9f\x45\xcd\x6f\xeb\x4c\xda\x80\xed"
	"\x2c\x91\x2f\x69\xa0\xb3\x97\x8c\xa4\xd6\x56\x01\x8d\x80\x0a\xa3\xa4\x69\x2a\xdc\x12\x1d\x10\x38\x59\x26\xe8\xde\x18\x4b\xa4\xc4"
	"\x22\x92\xa5\x0a\x68\xe4\x0f\x8c\xc5\x7b\x94\x58\x70\x1e\x8e\x1c\x25\xdc\x04\x19\x4e\x8c\xfc\x04\xb2\xca\x36\x4a\x78\xd2\x53\x3e"
	"\xa3\x84\xa3\xce\x48\xd4\xc3\x45\x1e\xda\x61\xa6\x4f\xa8\xa5\x24\x02\x05\x97\x14\x56\x02\x08\xaa\x90\xb4\xda\xea\x43\x9d\x08\x34"
	"\xbf\x67\x04\x5a\x42\x46\xa0\xe3\x6d\xb4\x3e\x82\xfe\x0d\xb0\xc6\x1e\x76\x4a\xd1\x5f\x6f\x28\x10\x40\x0b\xb8\x4f\x0c\x3a\xec\xb0"
	"\xb2\xae\x80\x28\x40\x84\x2c\xde\xf8\xd3\xf0\xe3\x8c\x5b\x0c\x9a\x0a\x85\xe5\xe1\x37\xaf\x78\x79\x42\x98\x02\xda\xb2\xc3\xff\x62"
	"\x77\x4c\xca\x10\x63\x4f\xb8\x21\x93\xf0\xdc\x84\xfc\xa8\x17\x24\x66\x1a\xeb\x13\x70\x40\x68\x85\x3b\x27\xc7\xe7\xe4\x01\x19\xc9"
	"\x4b\xd2\x18\xdd\xcb\xc6\xca\x1d\x10\xb2\x0a\xf8\x7e\xcb\xae\xc9\x20\xf8\xc2\x26\x1b\x38\xeb\x07\x4e\x58\xd3\x89\x18\xfc\xad\x3f"
	"\x23\x07\x12\x7d\xc6\x10\x84\x5e\xe6\x12\xfe\x79\x4d\x21\xda\xfe\x20\xc7\x68\x97\x14\x6d\xc9\x02\xf5\x95\x0b\x03\xa1\xe5\xd2\x0d"
	"\x5d\xc5\x7f\x78\xfc\x0a\xb1\xa6\xbe\x27\x24\xff\xfc\x5d\x0a\xa8\x4b\xe9\x0b\x35\x4f\x63\xd7\xf8\x15\x03\x34\x9c\x15\x40\x23\xe0"
	"\xa3\x00\x2d\x9e\x00\x8c\x01\xfa\x78\xa5\xf3\xd9\xb0\x88\x18\x36\xee\x93\x8f\x3f\x71\x1f\xae\xde\x5e\xbf\x37\xd5\xf7\x54\xa0\xb0"
	"\xc2\x82\x06\x7e\x0e\x0e\x20\x51\x1e\xf4\x69\xf7\xf7\xd4\x28\xd4\x66\xb2\x92\xfe\xee\x67\x4e\x43\x29\xf4\x79\xb0\xca\x76\x31\xce"
	"\x3c\x31\x9f\xbf\x44\x1d\xdd\x14\x78\x3d\xd6\x5c\x52\xb3\x46\x98\xbf\xda\x10\xa3\xec\xa4\x0b\x63\x28\xba\x6a\xb8\xcd\x7b\xfe\xf3"
	"\x92\x83\xda\x95\xe5\xaf\x43\x09\xca\x77\xd3\x8e\x72\xca\x87\xae\x2e\x93\x9c\x66\xd6\x39\x72\x3b\xef\x10\x89\xc2\x4d\x2e\xaa\x8e"
	"\xb5\x42\xd3\x0a\xd0\x5c\x69\x83\xa6\x40\x47\xc7\xc7\x0a\xda\x4c\xb5\x28\x42\xec\x58\xa9\x8d\x7e\x5a\x3a\xf0\xfd\x39\xc1\xaf\x36"
	"\x78\x61\x42\xcc\xfd\x8a\x3b\x59\x0a\x01\xef\xca\xf0\x63\xf6\x96\x86\xa7\x1c\xf6\x1a\xa2\x6e\x40\x2e\x83\xc9\x1c\x62\x0e\xbf\x02"
	"\xbd\x4f\xc3\xa0\xae\x8d\xfd\xe4\xfa\x8b\x32\x45\xe7\xa5\x59\x32\xe9\x99\x15\x11\xbf\xfd\x35\xf5\xa8\x19\x12\x62\xd4\xcd\x05\x97"
	"\x3d\x56\x59\x8e\xf3\x49\x25\x70\x33\xdb\x24\x21\x1f\x33\x56\x28\x30\xa4\x3c\xc4\x6b\x99\x40\x6f\xf4\x71\x56\x4e\xb6\xe3\x2e\xe9"
	"\xe5\xfe\x80\x08\xdd\x3e\xfc\x04\x5d\xc1\xdd\xbe\x8a\xd0\x33\x4f\x99\x1d\x30\x08\xeb\x40\xe7\x65\x30\x8c\xed\xc8\xcb\xa7\x73\x58"
	"\x04\xb0\x86\xbc\xce\xa7\x71\x53\x59\x64\x57\x2c\x9c\x96\x5c\x20\x6c\x35\xbf\x9a\xa1\x5a\x96\x7a\x87\x2c\xb0\x37\x67\xb9\x4f\xf7"
	"\x51\xbf\x03\x3d\x8c\xdb\x49\x50\x2d\xbc\x39\x0f\x00\x7e\x19\xca\xf7\x60\x5d\x98\xc5\x41\xbd\xea\x3d\x3f\x95\xdc\x3e\xa4\xd1\x0b"
	"\xd5\xb5\x32\xe0\x10\x0e\x9f\x3a\x7d\xb9\x12\x4b\x4c\xd0\x59\xf7\x0b\xcd\x97\x8f\xd9\x65\xf7\x10\xf5\xe2\xc7\x12\xd0\xcb\x99\xf7"
	"\xc7\x47\x79\xfe\x10\x33\xa8\xa3\xbf\x42\x5d\xc7\x03\x56\xcd\x27\xd1\xac\xd6\xe6\x99\x54\x54\xf6\x1a\xf4\xe4\x1d\xdb\xcb\x8b\x90"
	"\x58\xb3\xbf\xcd\xb8\xc5\x43\x2f\x1b\x2f\xf6\x27\x99\x19\x25\x12\x18\x1f\x17\xcb\x5c\xf2\x65\x7e\xba\x96\x99\xf0\xdc\x3f\x8d\x0b"
	"\xb5\x93\xdf\x52\xf7\x03\xed\x80\xe7\x74\x18\xaf\xaf\xeb\x0e\x67\xec\x28\x15\x03\x5d\x30\xb3\x4c\x81\xd2\xce\xce\x76\xd9\x46\x4d"
	"\xa3\xfe\x6b\x3a\x60\xfa\x01\x71\x9e\xb3\x0e\xd6\x66\xc3\x6e\x26\xb2\x20\xca\x29\x36\xb4\x24\x41\x39\x45\x9f\x3b\x95\xcc\x91\x6b"
	"\x47\x52\xb4\xaf\x3d\x88\x2f\x27\x1e\x05\xec\x49\x22\xbc\xc1\xd4\xe5\x80\x8d\x9b\x6e\x96\xbe\xec\x17\x2f\x3f\xb5\xa2\x0e\xa2\x5e"
	"\x41\x69\x57\x75\x1e\x62\x28\x7a\xb3\x34\x88\x11\x53\x23\x25\x60\x53\xc3\x8f\x18\x10\x46\x41\x5b\x68\x24\x57\xc6\x46\x01\x16\x90"
	"\xe2\x63\x08\x9f\xbd\x1c\xe7\x3f\x9e\xb5\x1f\xb7\xaf\x5f\xde\x1d\x6b\xcd\xb4\xdf\xa8\xd9\xfa\x9f\x75\x3d\x9b\xc3\x97\xa6\x76\x9b"
	"\xe0\x00\x42\xf7\xfc\x76\x3d\xca\x26\xcb\x1c\xce\x05\x39\xfb\xa9\x89\x47\x90\x80\x93\x3d\xe8\x8d\xd1\x41\x47\x3f\x11\xcd\x2a\xd2"
	"\x32\x39\xb8\x7f\xbe\x64\x03\x86\x01\x3d\x03\xdd\x74\xba\x98\x55\x69\x55\xf3\x11\x9a\x88\xa1\x6b\x40\xd2\x84\x03\xa1\x91\x22\x4d"
	"\xec\x16\x04\x35\x48\x93\x22\x29\x55\x1b\x15\xeb\xd1\x20\xec\x86\x0b\x49\xd5\xa1\x22\x8f\x6b\xd9\x0c\x31\xde\x9f\x94\xbd\x0e\x4d"
	"\xd8\x8c\xfb\x3b\x09\x53\xcd\x63\xc1\x66\xfd\x25\xec\xe3\xdc\xf7\x92\xd1\xbe\xda\x26\xb6\xbe\x4f\x9a\xed\xf8\xc8\x0c\x67\xd1\x40"
	"\x88\x84\xec\x8b\x23\xa0\x18\x5a\x2e\x23\x20\xd4\x42\x1e\x6b\x97\x39\xae\x14\x65\x9d\xd9\x03\x33\x52\x21\x3e\x18\xeb\x65\xba\x5e"
	"\x8b\xe4\x66\xd8\xfd\xb4\x1f\x06\x33\x90\x5e\x86\xb3\xa7\xf4\x23\x4b\xfb\x85\xbe\x5c\x10\x36\x2f\x71\xf5\x5e\x59\x04\x57\x83\xf5"
	"\xfc\x2c\xcf\x7b\x84\x35\xd0\x0e\xe1\x2d\x06\x51\x49\xf8\x62\x30\x14\xa8\x9d\xd0\xe7\x17\x87\x83\x1a\x82\xae\x43\x26\x94\x45\x66"
	"\xe2\x44\x81\x63\xdc\xe1\xa8\x3a\x32\x92\xe9\x6e\x15\x94\xc3\xd7\x27\x49\x8e\xcf\x6f\xfe\x29\xc7\x0b\x34\x29\x4a\x76\xa0\x61\x32"
	"\x3a\x8d\x6b\x3b\x15\x0a\x57\xbd\x2b\x21\x82\x90\xc2\x6b\xb1\x14\x1a\xdd\x78\xe2\xb6\xcf\xc3\xc0\x7d\x05\x4b\xd5\x8a\x9d\xe9\x38"
	"\x15\x0d\x4e\x82\x72\x19\x9a\xbf\x13\xaf\xc8\xdf\x4f\x62\x8e\xbc\x0b\x1c\x85\xe8\x9b\x6c\xa4\xe7\x88\xd5\x09\xcc\x4e\x09\x4d\x4e"
	"\x61\x8c\x93\x98\xd6\xd0\x3f\xb2\xd6\x4c\x91\x38\xa2\xa1\xea\x00\x67\x3c\x1e\x2f\x0c\xa2\x55\x3c\xcd\xc7\x35\x01\x8f\x2c\x89\x46"
	"\x95\xc1\x5e\x50\x3b\xa7\xc4\x10\x89\x03\x45\x10\x4d\xb8\x11\x90\xf2\xd4\xba\xc6\x17\x0e\x0b\xa2\x3f\x4b\x79\x5a\x88\xa6\x50\xc6"
	"\x47\x5a\xb7\x0f\x13\x83\x0f\x86\x5e\xe7\x61\x9d\x50\x64\xd5\x0f\x9d\x85\x5b\xb4\x45\x13\x4a\x0b\xf5\xfc\xf0\x60\xf3\xc5\x41\x71"
	"\xe8\x03\xc2\xa1\xec\x19\xd2\x5c\xfb\xd3\xa5\x54\x25\x66\x6b\x5c\x48\xc1\xb7\x4a\xde\x56\x12\xb0\x6b\x15\xf8\x2d\x11\x06\x40\x5e"
	"\xf8\xd5\xda\x50\xe3\x05\xfd\x9a\xf6\x30\x56\xb7\x97\xa2\xbf\x2c\xc7\x85\xc3\x44\xde\x38\xd7\x56\x2d\x23\x85\xec\x08\x7c\x69\xe1"
	"\x98\x27\x1f\xe0\xfb\x76\x18\x12\xf3\xca\xfb\x70\x3f\x0e\x78\xa3\xfb\xc0\x64\xf2\xed\xc6\xd3\x9c\x68\x48\xa0\x74\x1e\x12\x09\x33"
	"\xdf\xf3\x19\x43\xec\x6c\x8f\xbf\xb7\x74\x43\x52\x5b\x30\x79\x03\x93\xeb\xc8\x48\x5e\x86\xb2\xb4\xb5\x9a\x05\x40\xa4\x32\x10\x48"
	"\x45\x21\x9f\xab\x3f\x5c\xcf\x48\xe6\x88\x97\x13\x1e\x4a\xb3\xf7\x3c\x16\x97\xfa\x5f\x6a\x09\x84\x8c\x87\x3e\x41\x17\xac\x62\xad"
	"\x57\x86\xda\xe1\x8d\x1f\x6b\xf6\x17\xec\x58\x7b\x94\x28\x09\xa9\xfc\x10\x08\x6e\x02\xd7\x70\x04\xfc\x26\x57\xd9\x58\xe1\x9c\x32"
	"\x20\xe2\x22\x38\x05\xdf\xbe\x86\x11\x15\x05\xf3\xf3\x48\xd7\x6e\xdd\xf8\x40\x46\x67\x35\x1a\x73\x93\x38\x57\xe0\x7c\x68\x4a\x1a"
	"\x08\x8b\xb8\x90\x21\xb9\x6b\x7d\xfb\xe7\x95\xfc\xa1\x8a\xe7\x45\xdd\x40\x8b\x38\x09\x47\x87\x3e\x3f\xed\xe6\xf5\xe5\xbb\xe1\xf3"
	"\x15\xa9\x18\x1b\x0a\x84\xb5\xa5\x76\x8d\x6e\x9a\x37\xbd\x74\xa1\x33\xa1\xdd\x8e\x54\x29\x23\xfe\x2f\x4c\xf6\x7e\x5f\x42\x0a\x46"
	"\xa5\xac\xd2\xc8\x3d\x8c\xee\xea\xd1\x40\x9b\xd0\x07\x43\x73\x83\x92\xd1\xda\x5b\x40\xa9\x89\xc8\x82\xb1\xaa\x7f\x5c\x6e\x09\x87"
	"\x8d\x9f\xb0\xc1\xff\x3b\xfb\xc5\xb1\x55\xb1\xb4\xfa\x57\xdf\x52\x21\xef\xfd\xe6\xe0\xbe\xa4\xd5\x34\xd1\x9c\x50\x7f\xdf\x4d\x2c"
	"\xb3\x1f\xe3\x2d\x28\x32\x47\x45\x3d\x91\xd2\x44\xa7\xbd\x2c\x08\x31\xee\xe9\x40\x53\xa7\x5f\xd0\xfb\xbe\x71\xf8\x27\x14\xf4\xb9"
	"\x93\x21\x63\x94\x5e\x45\x92\x55\x6a\x49\x1b\x0d\xc0\x8f\x2e\xc3\x2b\xc5\x62\x82\xb0\x15\x89\x42\x4b\x81\xb1\x1c\xee\x3c\x2a\x4d"
	"\xd4\x78\x64\x88\x3c\xbc\xe9\x3e\x9e\xf5\x63\x07\x03\x35\xc7\xde\x97\x1a\x48\x1f\x72\x14\x65\x8f\xa5\x67\xc6\x85\xe2\xf5\xb8\xb2"
	"\x34\x3c\xdc\xb2\x47\x48\x46\xfd\xe3\x23\x70\xd4\x6d\xee\xca\xbb\x63\x95\x6c\x4c\x59\x07\x6d\xd0\x7b\xc2\xa6\x0d\x9c\xfe\x3f\x6c"
	"\xd5\x1d\x55\x27\xd4\x26\x1f\x04\x0f\x29\x11\x69\x6a\x07\x6d\xd2\x3c\x16\xeb\x5c\xee\x83\x03\xbd\x30\x3e\xaa\xd8\x21\xa7\xb6\x27"
	"\xae\xbe\xc4\x82\xdb\x0b\x84\xfc\x91\xd9\xdb\xd1\x02\x3a\xad\x15\x31\x50\x6e\xf6\xa8\xef\x0a\xf2\xf9\xe7\x5e\x8e\xae\xa5\x9e\x3e"
	"\xd1\x01\x03\x5f\xb2\xfe\xae\x62\x41\x85\x38\x0f\x8f\x2b\xe9\x23\x1d\x71\x87\xd9\x97\xc7\x41\xae\x3b\xe6\xb7\x09\xac\xb8\x00\x6d"
	"\x0e\x20\xd9\x55\x7f\x44\x5e\xbf\x65\x89\x2c\x5b\x24\x16\x25\x0a\xaa\x38\x52\x12\x75\x70\x2b\x28\x75\x9e\x73\xbe\x91\xc6\x21\x78"
	"\x21\xb4\xb9\x86\x3c"
;