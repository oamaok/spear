#include "GameShaders_macos.h"
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
	{ { 2 }, { 1,2,3,4 }, {  }, 1910, 7348 },
	{ { 2 }, { 1,2,3,4 }, {  }, 9258, 7348 },
	{ { 2 }, { 5,2,3,4 }, {  }, 16606, 8018 },
	{ { 2 }, { 5,2,3,4 }, {  }, 24624, 8018 },
	{ { 3,4 }, {  }, { 1,6,7,8,9,10 }, 32642, 2089 },
	{ { 2 }, { 1,6,7,8 }, {  }, 34731, 7296 },
	{ { 2 }, { 1,6,7,8 }, {  }, 42027, 7296 },
	{ { 2 }, { 5,6,7,8 }, {  }, 49323, 7966 },
	{ { 2 }, { 5,6,7,8 }, {  }, 57289, 7966 },
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
	"\x28\xb5\x2f\xfd\x60\xe7\xfd\x5d\x69\x00\x9a\x72\x08\x15\x2b\xd0\xac\xa6\x0c\xa0\x21\x6e\x60\x29\x30\x28\x92\x4f\xe4\xc5\x40\x98"
	"\x89\xe2\x58\x57\x49\xcf\x99\x0d\x31\x68\xcb\xba\x2c\x8b\xeb\xba\x26\x11\x74\x3c\xc0\x17\xb3\x4a\x7d\x39\x42\x01\x4e\x01\x43\x01"
	"\xd4\x52\xfa\xde\x74\x4f\x8d\x8f\xb5\x0f\x8c\x0f\xce\x85\x5f\x26\xad\x8f\x85\x96\xc2\x27\xca\x0a\x25\x85\xf4\xa9\x54\x5a\xc7\x3d"
	"\xf3\x58\x6b\x94\x5d\x61\x6b\xc2\x13\xb0\x67\x7e\xc2\x57\xa9\x9b\x53\x3e\x6b\x65\x75\xd3\x4d\x57\xa3\xb5\x91\x52\xcd\xaf\x8e\xf9"
	"\x32\x69\x94\x11\x3a\x49\x63\x8c\x11\x56\x48\xa1\xb5\xd6\x4a\xe9\x24\x44\xe0\xb1\xf6\x91\xbc\x38\xe7\x7d\xe3\x1c\xf7\x0d\xd6\x2a"
	"\xea\x4e\x90\x01\x03\xba\x75\x23\x0f\x3d\xb6\x60\xaf\x20\x5d\xcc\x33\x7a\xe3\x17\xeb\x1c\xb1\x2b\x30\x24\xdc\xfa\xc6\xb2\x44\x56"
	"\x76\x29\xd3\x6d\x2d\x8a\xa2\xe8\xb1\xce\x93\xbe\x8b\xcc\xf1\x3e\x84\xf7\xe5\xbb\x0d\xf7\x0b\x23\xf3\xde\xef\xd5\xf9\xe2\x62\x22"
	"\x9a\x24\x2f\x67\x96\x72\x72\x45\x93\x87\x31\x63\xc0\xae\xb1\x44\xdf\xfe\xa6\x4b\xf1\xad\x1d\x17\x17\x8b\xd3\x90\x3d\x35\x8e\xad"
	"\xcb\xc2\xe2\xf5\xce\x3d\x5b\xbb\x8a\x26\x1f\xfb\xb5\x23\xa7\xf3\xa5\x26\x34\x1c\x55\x9e\x10\xec\x0a\x69\xe2\x9b\xf2\x12\x93\xfd"
	"\xee\x29\xf2\xff\xdf\x90\xd0\xa0\xf8\x8e\x58\xfa\xf2\x8a\xef\x90\x8b\x77\xab\xd1\x70\x3c\x86\xdc\xca\xc2\xd4\xd5\x7f\x35\x1a\x05"
	"\x32\x9c\x06\x89\xca\x56\x01\x8c\x3c\x49\xdd\x8a\xa6\xdb\x31\x45\xe0\xb1\x74\x4d\x10\x9d\x05\x34\x1a\x01\x2c\x5c\x65\x7d\xf7\xcd"
	"\x2b\x06\x89\xce\x96\x95\xa9\x06\x0c\x01\x5e\xc2\x7c\x6b\x49\x7e\x48\xee\xd6\xd6\x92\x9b\xa5\x97\xe8\x3b\x41\xfe\x52\x92\xcb\x2b"
	"\x01\x01\xae\x46\x27\x63\xa4\xd5\xba\x58\x5f\x74\xdc\x43\x73\x78\x3c\xf3\x0f\x6d\xca\xf8\x98\x10\xf2\x11\xbf\x81\xfe\xc1\x45\xbe"
	"\xf9\x26\x12\xc9\x12\xe4\x15\x22\xbf\xd3\x05\xc1\x95\x15\x3d\x38\x30\x38\xe0\x1c\xd7\xf0\xd0\x17\x78\xf1\x2d\xcc\x57\xf9\xc8\x37"
	"\xa1\x83\x55\xce\xfa\x62\x85\xce\x43\x6d\x8d\xa5\x05\xe8\x17\x9e\xe1\x21\x20\x1e\x7a\x87\x88\x88\x47\x04\x26\x91\x30\xdf\x5a\xeb"
	"\x3c\x82\xf5\xcc\x43\xff\xc0\x24\x12\x78\x33\xaa\xe9\x42\x3c\xbe\x89\xe8\x5b\x92\x68\x0e\x12\x24\x88\x1c\x1e\xef\x58\x16\x89\x35"
	"\x17\x79\x86\x6f\xed\x17\xde\x41\x28\x31\x86\x77\xce\x83\xe8\x5b\x7a\xbc\xc2\x37\x2e\x02\x01\x92\xa7\x0c\x08\x45\xf4\xf0\x71\x71"
	"\xcd\xc2\xe2\xf9\x9e\x36\x24\xda\x2f\x2f\xe4\x63\x71\xfb\x75\xe8\x3a\xef\x2a\x4e\x5c\x8e\xa9\x0b\x89\x24\x0c\x01\x72\x2b\xf8\x9c"
	"\x74\xab\x28\xaa\xac\x07\xab\x67\x48\x72\x04\x75\xe9\x09\x63\x79\x02\x40\xbf\x36\xf4\x14\x87\xe2\x34\x95\xcc\x69\x30\x27\x3f\xef"
	"\x45\x79\x72\x4d\x2e\x0a\xf3\x4b\xd3\x2c\x6b\x73\xc8\x43\x95\x0c\x73\x1a\xf3\x0e\xe4\xe8\xfc\x90\xd3\xaf\x3d\x41\xc2\x43\x1c\x47"
	"\x22\x17\x86\x85\xc5\x3f\x34\x7a\x05\x7d\x9b\x8b\x0b\x9d\x23\x2c\xb9\xdd\xca\xdf\x30\xdc\x39\x9e\x38\x16\xda\x50\x19\x35\x48\x2b"
	"\xf0\x45\x70\x66\x2f\x72\x2c\xd0\x53\x19\x39\x30\x67\xf2\xd0\x53\x97\xdf\xea\x97\xc9\x2b\x74\x4b\x6f\x48\x9c\xd0\x98\x30\xe1\xe3"
	"\xe4\xf3\xa2\xa4\x4f\x9c\xd6\x49\x07\x7e\x33\xe1\x33\xdf\xac\x01\xf4\xa9\x35\x56\xeb\x60\xb4\x56\x56\x0b\x1b\x4c\x62\x93\x5f\xf8"
	"\x86\x9f\x40\xcc\x4f\xe0\x43\x8f\xf9\x65\xed\x0a\xc3\xd5\xa5\xb0\xde\xfd\xa5\xac\xcd\xa4\x51\xd6\x68\x69\x84\xa4\x8b\xf0\x3d\x94"
	"\x3e\xd4\x3e\x47\xad\x75\xee\x49\x07\xed\x9c\x30\xd2\x67\x3e\x4f\x32\x76\xc2\x67\xa9\x9c\x54\xce\x38\xa5\x8d\x16\x3e\x14\x5a\x07"
	"\xbe\x2a\x1f\x74\x10\xc6\x27\x3e\xd1\xc2\x09\xe1\x84\xd5\xc5\x17\x9d\xc3\x90\x70\x8a\x5d\x31\x61\x42\xf9\x1e\xe9\x9e\x85\x4f\xc2"
	"\x5a\xab\x95\xce\x39\x6e\xc9\x1c\x89\x66\x61\x5c\xe6\x5b\x6b\x0b\xcb\xf3\xcd\xc2\x3c\x33\x22\xbe\x08\x02\xe2\xf1\x88\xf8\x46\x62"
	"\x81\xf8\xe6\xf1\xf8\x85\x8f\x68\xb8\x92\x1f\x7a\x88\x8f\x0d\x6f\x80\x5f\xc7\xb2\x2c\xfc\x43\x65\x8d\xf6\xbd\xe7\x3d\x67\xe5\x73"
	"\xcf\x71\xed\x1b\xee\xa5\x71\xef\x37\x92\x31\x67\x99\xf0\x04\x4f\xdf\x2c\xdf\x2c\x7c\x56\xc1\x9c\xf6\x4d\x73\xa0\x67\x24\xbe\x37"
	"\x02\x0c\x57\xc4\x9b\x0f\xc0\xe9\x52\xf0\x10\x10\x60\xa0\x8f\x40\xc1\x5d\x8b\xd3\x65\x61\xbe\x34\x40\x1f\xb1\x34\xad\xef\x08\x14"
	"\xdd\xca\x13\xcf\x00\xfd\xed\x13\x96\x28\xd6\x95\xa5\xcb\x20\x4b\x32\x5f\x30\x7d\x8c\x11\xc2\xf7\x1c\x17\x0e\xf4\xee\xf6\x2b\xbd"
	"\xe4\x53\x45\x51\x70\x3c\x55\xd4\xb5\x1c\xa2\x94\x79\x02\xca\x96\x6e\x86\xf5\x05\x08\x48\x11\xf3\xab\x63\xb8\x3f\xf4\x45\x93\x57"
	"\xc0\xd6\xc5\x78\x85\xae\xf3\x43\x62\x62\x02\xf7\x7b\x4d\x77\x82\xed\xac\x01\x94\xec\xd9\x67\x33\x8c\x92\x55\x59\x65\x45\xd7\x55"
	"\x85\xb0\x82\x12\xdd\xbb\x44\x77\x48\x71\xad\x1d\xc5\x35\x7c\xdf\x2c\x8d\xb5\x2b\x0a\xa5\x52\x71\x10\xbe\xb7\x64\x19\x84\xef\x61"
	"\x0e\xf3\x98\x83\x65\xe7\xc5\x88\xb2\xf9\x12\x99\x5f\xb0\xfc\xa2\x57\xa8\x10\x46\xaf\x80\x29\x16\x86\x64\x49\xe7\x08\x9f\xce\x10"
	"\x28\xa0\x7d\x16\x18\x2f\x2d\xac\xf7\x01\x4a\xe6\xdc\xe3\xdc\xd3\x64\xd8\x8b\xb0\x36\xf6\xc9\x0a\x65\x84\xd2\x3e\x49\x9f\x29\x23"
	"\x7c\xd1\x8b\xf0\x45\x48\x2b\xad\xb4\xd2\xda\xbf\x79\xd6\x37\xce\x7a\xdc\x7b\xef\x71\xaf\xd2\x24\x7d\x6d\x7e\x7d\x93\x61\x4e\x03"
	"\xbd\xb3\xa6\x6f\x2a\x98\x53\x4d\x1b\x7d\x9c\x9e\xe1\xb3\xc9\x4d\x4d\x67\xea\x4d\x74\x6a\x7c\xef\x29\xe2\xa6\xea\x54\x0f\x7d\x36"
	"\xbd\x81\xec\x72\x7e\x23\x09\x56\x14\x94\x0c\x9a\x51\x94\x31\x8d\xce\x32\x3e\x24\x1c\x13\xf2\xa1\x05\x02\x8e\xce\x77\xa5\xe1\x60"
	"\x9e\x79\x0a\xf4\xcd\x32\x67\x7d\x28\x7c\xa4\x85\xd1\xc2\x86\x65\x17\xc1\x1c\xe8\xf1\x60\xc9\x74\xc6\x1c\x46\xe7\xd6\x24\x3e\x56"
	"\xd0\x4f\x40\x0f\xc1\xba\x0a\x24\xf3\x15\xe8\x0d\xc4\xa5\x81\xfe\x41\x83\xcf\x7c\x86\x39\x4b\xd3\x24\xfa\xa5\x1e\xca\xd6\x75\xd6"
	"\x38\xc2\xe7\x5a\x84\x60\x5d\x0a\x3e\xeb\x4e\xdd\xc0\x96\x30\xc6\x74\xa1\x85\xc5\x37\x5d\xe3\x88\x8d\x93\x52\x29\x1f\xe9\x1e\x3b"
	"\x9d\x73\xf2\x3d\x18\xdf\x7b\x1e\x4a\x6d\x84\xb4\x4e\xc7\xd5\x17\x83\x58\xa8\x42\xe3\x18\x55\x08\xd1\x08\x00\x00\x00\x04\x20\x00"
	"\xc2\x09\x10\x08\xc3\x69\x96\x44\xad\xc7\x32\x12\x80\xb1\x50\x92\xa3\x40\x8a\x23\x39\x48\x19\x65\x0c\x31\x06\x88\x00\x00\x48\x00"
	"\x20\x00\x00\x0c\x00\x08\x09\xdc\xe0\x0e\x94\xde\xb6\xb3\x13\xf2\xc6\xef\x4f\xbc\xd7\x18\x78\x4b\x66\x7f\xcf\xd7\x71\x06\x2a\xad"
	"\xde\xc6\x73\x2d\x61\x60\xed\x07\x18\x6f\x6b\x8b\xd7\x16\x78\x5b\xc1\xbe\x9b\x53\x0d\x32\xda\x71\xc6\x16\x08\x3b\xa8\x51\xf1\xb4"
	"\x16\xdb\x9b\x36\x76\x6b\xbb\x6f\xc3\x6d\x3a\x26\x74\x4b\x84\xd1\x5b\xa7\x41\x26\x25\x44\x6b\xb7\x42\x8d\x26\xe4\x89\x58\x49\xa1"
	"\x6d\xac\x96\x49\x45\xcd\x84\x78\x37\xae\xbc\x4c\x18\x26\x5a\xfa\xce\xc6\x4f\xb0\xe4\xa5\x4c\xe8\xcb\xb1\xb7\xc8\x84\x1e\x6a\x82"
	"\x06\x27\x6c\x6c\xb5\x0b\xe3\x77\x68\xad\x8a\x0d\x78\x8c\xb4\x10\x07\x5a\xc0\x47\xc4\x5d\x5b\xb1\x50\xdb\x80\xbf\xff\xe7\x80\x7f"
	"\xc2\x1c\xf0\x81\x3b\x43\xcb\x03\x2e\x70\xae\x39\xbe\x7e\x6a\x95\xbe\x93\xf7\xff\x6c\xbf\xd2\x64\x9a\x39\xe6\x49\x55\x8d\xbe\xd5"
	"\x2f\x95\x01\xee\x89\xcf\x6a\xf7\x8c\x28\x28\xa7\xe1\xc2\x7a\xe0\x37\x0e\x5e\x1a\x63\x5a\xff\x41\xbb\xe3\xf7\xec\xde\xa7\x95\x07"
	"\x88\x0c\xeb\x55\x3c\x6e\x58\xbd\xfb\x69\x44\x4a\xc6\xf5\x14\x2f\x1e\x24\x56\xbd\x22\x7c\xc2\x02\x96\x5e\xa2\x76\x31\x1b\x3f\x7a"
	"\x39\x59\xc9\xba\x57\x40\x9b\xef\xe1\xed\x18\x1b\x12\xd4\x66\xff\x03\xe7\x4c\xa9\x77\x9a\xa1\x64\xd2\x11\x5e\x49\x72\x25\xe9\xed"
	"\x8d\x73\xf5\x4c\x36\xe6\xbb\x32\xc5\x4f\x9b\x5d\x1d\x23\xa1\x53\x14\x54\x96\xc7\xaf\x42\x19\xe8\x32\x17\x6e\x75\x6f\x5e\xaf\xdc"
	"\xa3\xdc\x48\x3d\x2c\xa8\xc2\x63\x48\xd6\x7f\x4a\x75\xed\x7c\xb0\x82\x14\xf8\xe5\xfd\x1a\x4e\xe1\x3f\xd7\x7e\xf5\xb7\xb8\x02\xd0"
	"\xfb\xf5\x41\x20\x67\xa4\xa1\xc4\x15\x5e\x8e\x57\x1e\x7f\xe6\xee\x31\xb5\x1d\x75\xb7\xaa\xff\xa9\x40\xa4\xdb\x44\xa5\xd4\xdc\xf8"
	"\x80\xd0\x1e\xb2\x96\x5d\xdf\x49\xb1\x9d\x68\xca\xfa\xf4\x56\x95\x89\x73\x89\xe9\x51\xdb\x8a\x38\xfa\xca\x9e\x43\x8f\x3e\x21\x18"
	"\x7d\xc5\xe8\xfa\xe6\xbc\xa2\xf9\x46\x43\xff\xed\x90\x34\x5d\xe3\x00\xb9\x27\xfa\x4a\x70\x97\xa7\x3c\xe4\xd1\xf0\xe3\xa8\xa0\x8f"
	"\x30\x0d\x3f\xbf\xe6\x30\x7c\x0e\x17\x39\x06\x9b\xc2\x2f\x7b\x83\xd2\x09\xb2\xa7\xa0\x08\x98\x9b\xba\x22\x7c\x1d\x2a\xcf\x3b\xed"
	"\x86\xde\x0e\xd1\x5b\xec\x14\xe8\xd5\xf3\x88\x46\x9b\x41\x2f\x8a\x10\x17\xd6\xb6\x0c\x3f\x9e\x6e\xee\xee\x5c\xcc\xbb\x35\x9d\x26"
	"\xf3\x73\x3f\xdf\x9d\xb0\x40\xc0\xbb\x9a\xfe\xb8\x44\xa1\x0f\x19\x47\xcb\x86\xf4\xf3\x8f\xb7\xb5\x70\x45\x4c\xe7\x0b\xb9\x77\x0f"
	"\x07\x12\xfc\xb1\xfb\xd1\xbb\x21\x53\xce\x71\x3a\x2d\x93\xf8\x18\x45\x6a\xd9\xd2\xa5\x15\x99\xe1\x47\x47\x9d\x1f\x71\x99\xad\x6c"
	"\x15\x46\xc3\x27\xe4\x57\x98\x63\xa2\xa2\x84\x59\x95\xce\x3c\xe5\x81\x78\x82\xf1\xff\xc6\xe0\x65\x35\x81\x6d\x04\x26\x5d\x6e\xbe"
	"\x77\xcb\xdf\x0e\x6f\xd1\x05\x30\xd9\x55\x6e\xd9\xd6\xa3\xbc\x83\x8f\xd9\xdd\xef\xf3\xf0\x31\x6c\x49\xda\xb3\xd0\xf9\xe1\x03\x1c"
	"\x42\x4e\xcc\xa7\x8b\x2a\xd9\x55\x4d\xec\x08\x4a\xc6\x11\xb6\x9a\x31\x98\x39\x2d\xcb\xbd\x4f\x16\x31\x9b\xb3\x00\x60\xe7\xa8\xa7"
	"\x03\x1d\x0c\xf4\x24\xa8\x96\xdf\x9d\x07\x98\xbe\x92\xf2\x2d\x8a\xee\x60\x71\xa3\x21\x78\x2f\x4e\x84\x5b\x14\xa9\xba\x2e\xd9\x21"
	"\x8a\xd4\xc1\x45\xa1\x4e\x5e\xcc\x62\x89\x11\x74\x32\x60\x6a\xe7\xeb\x04\x0c\x4a\x5d\x61\xfd\xbe\x63\x6d\xf6\x02\xcf\x43\xfd\xa3"
	"\x88\x7c\xaf\x0c\xea\xf4\x5f\xa1\xa2\x13\x96\xab\x2a\xca\x99\xae\x17\xf0\xf0\xa6\xa7\x17\x0f\x1e\xbf\x63\xe7\xc5\x90\x90\xbf\xb9"
	"\x45\x3e\xc4\xf0\x30\xd0\x46\xc5\x9d\x14\xcc\x38\x28\x9c\xea\x0e\x99\xca\x11\xe9\x70\x8b\x5a\x57\x1c\x52\xea\x0f\x55\x1a\x8d\xd4"
	"\x6f\x91\xef\x7c\x22\x82\x4b\x3b\x6f\xae\x8c\xba\xa5\x65\x9e\x34\x35\x74\x97\xa9\x8a\x4c\x27\x69\x34\xde\x76\x9f\xad\x14\x8e\x36"
	"\xa1\xc9\x65\xd6\xe7\x66\x5a\xb0\x0d\x97\x5e\xc3\x2d\x13\xb9\xd6\xa1\x61\x1c\xfa\xdb\x2c\xf5\x45\xa7\x29\x16\xf5\xef\x2a\xb8\x29"
	"\x9c\x48\xdb\xc4\x5d\xea\x1d\x2d\x89\x35\x4e\xa1\x29\xdb\xa5\x9f\xb2\x59\xbc\x6d\xae\x59\xaf\x14\x77\xc2\x08\xe6\x51\x0f\x78\x54"
	"\x50\x05\x1b\x8a\xab\x0a\x73\x89\x6f\x15\x76\x33\x65\xaf\xed\xd4\xc3\x0e\xdc\x14\x5a\x8e\x09\x12\xf7\xaf\xf8\xe4\x16\xd0\x88\x9c"
	"\x04\xc1\x0c\x11\xe9\x36\xa2\x64\x0b\xb7\x6b\x5e\x10\x13\xd6\x8a\x70\xde\x6e\xb1\x7a\x9f\x75\x24\x92\x86\x9d\xc2\x77\x63\x70\x00"
	"\xb9\xef\xb9\x8b\x93\xb2\xa5\x65\x2e\x47\x82\xe0\x7e\xba\xe3\x91\x37\xe0\x64\x0c\x7a\x63\xf4\xcc\xd1\xb3\xa3\x32\xc7\x63\xcc\x02"
	"\x87\x61\x25\xa9\x00\x43\x1d\xdd\x99\x52\x3a\xbd\x9b\x55\x78\xda\xca\xe5\x13\x79\xd4\x57\x19\x64\xab\x88\x58\x4e\x40\x36\xfb\x08"
	"\x3b\xd2\x8f\xad\x52\x5a\xb4\xee\x0a\xf2\xb1\xa4\xb9\x45\x95\x53\xe2\xa3\x10\x7b\xd1\x33\x43\x0c\x33\x27\x41\x36\x1b\x7b\x3f\x13"
	"\x79\x37\x1b\xcb\x22\x35\xd8\x58\x7d\x29\x66\x1c\xfb\x6c\xee\x98\x55\xdb\xa4\x6f\xef\x42\x56\x1d\x9f\x9a\xdc\xe8\x98\xa5\xe8\x41"
	"\xf5\x6e\x53\x54\xa8\x45\xbb\x25\x23\x92\xf9\x14\xfa\x73\x42\x2c\x4e\x3c\x13\x1d\xec\xf7\x7d\xb8\x68\xeb\x5d\xba\xde\x46\x72\xed"
	"\xaf\x3d\xa6\x1f\x36\xa8\xb1\xe8\xe1\xb9\x3f\x18\x89\x20\x52\x91\x0c\x0d\x9f\xf8\xf6\x45\x3d\xdb\x2d\x62\x24\x7f\xa1\x55\x14\x37"
	"\xfb\xa0\xdb\x41\x6c\xa0\xc0\xa6\x77\xf1\x47\x75\x00\x05\x47\x1e\xe8\x96\x4f\x7c\x7e\x66\xa5\x25\xc1\x18\x0a\xaf\x0a\xa3\x00\xa0"
	"\xc6\x39\xc3\x61\x43\x4a\xfe\x85\x74\x73\x53\xf3\x7f\x7b\x41\xd3\xba\xe5\xff\xa1\xe5\xbe\xab\x03\xa9\x8d\x69\x47\x7c\x43\xb3\x0a"
	"\x26\x12\x41\x9a\x62\x88\xaf\x43\x64\x27\x29\x23\x45\xde\xcd\x55\xd5\xe2\x65\x53\x1b\x78\xa9\x22\x12\xc2\x41\x3a\x51\x80\xb0\xb2"
	"\x6b\xf7\x95\xca\xb0\xa5\x14\xdc\xaf\x75\xca\x10\x50\x28\x0b\x29\xe9\xe0\x25\x06\x41\xb7\x46\xc6\xba\xdc\xe7\xb1\x80\x9e\x59\xbf"
	"\xe1\xeb\xbc\x16\x39\x91\xa1\x11\x58\xaa\x35\xa0\xa0\x0d\x86\xed\x5b\x89\x6f\x2f\x40\x80\x77\xaa\x84\xe4\x93\xe2\x84\x66\xc1\x88"
	"\xcd\x3c\x7d\x5f\xa3\x25\x87\xcf\xd4\x2b\xa5\x81\x16\x4e\x73\x0c\x99\x23\xcb\xed\x5d\x8b\xa1\xc1\x87\xde\xfe\x90\x54\xe0\xb1\xaa"
	"\xf5\xce\xde\x32\x5a\xbe\x41\x43\x8b\x3b\x81\xde\x70\xc1\xc5\xdd\x08\x54\x06\x92\x75\xa4\x66\x60\xae\x15\xd4\xd5\x86\x12\xc1\x65"
	"\x00\xb5\x12\xe7\x2e\xc8\xd1\x02\xba\xb7\x98\xbf\xdd\xc4\x1c\x87\x17\x7d\x85\x16\xbb\xf1\xfa\xbf\x86\xcf\x81\xf1\xed\xa5\x8c\x5f"
	"\x52\xb8\x70\xc4\xca\xdb\x7d\xdb\x2a\xe4\x89\x90\x1d\xe1\x2f\x26\x18\x83\x54\x04\x7c\xab\x0e\x43\x7a\x5e\x79\x70\xf4\xe3\x0e\x0f"
	"\xbb\x0f\x4d\x26\xb1\x6e\x2c\xed\xc4\x4e\x03\x45\x90\x1b\xa4\x3a\x63\x90\x0f\x37\x44\x8c\xbd\x34\xfd\x29\x6d\x50\xc0\xf2\x7d\x36"
	"\xbc\x1d\x56\xa6\x27\x64\x08\x56\xc3\x8d\x59\x26\x23\xd5\x3c\x62\x8a\x28\xa4\xac\xfa\x84\xf5\xb2\x23\x8e\x38\x72\xe1\x0b\xd9\xac"
	"\x10\x81\xc5\xab\xfe\x97\x72\x02\x21\x23\xda\x33\xd1\x45\x6f\xa6\xe2\x9e\x85\x76\xe4\xfc\x7c\xc6\x96\x60\xc4\xda\xe3\x41\xa9\x91"
	"\xf2\x94\x40\x05\x4d\x80\x8c\xa6\xf0\x5f\xdc\x96\xcd\x82\x73\xb6\xc0\x34\x97\x20\x2a\x68\xf7\x28\x47\x54\x35\x18\x8d\x0b\x0a\x25"
	"\x7e\x4d\xcd\xec\xf0\x05\xda\x62\x1a\x8f\xd3\x0e\x2e\xa2\xe6\xa1\x81\x7e\xc4\x2e\x19\xda\x5d\xf8\xdb\x47\xaf\xf4\x0c\xaa\x3d\xaf"
	"\x83\x0d\x14\x82\x70\xc6\x1d\x71\xf8\x43\xb7\x40\xba\x7c\x41\x38\x17\x92\x8a\x3b\xa8\xe0\xac\xcd\x2f\xd7\xa2\xd4\x40\x70\x33\x39"
	"\x9d\x1d\x76\xcb\xae\x25\x89\x78\x0a\xb2\xd2\xfc\xce\x9f\x0a\xd2\xed\xb8\xf8\xc8\xbd\xe5\xdc\xd5\x3e\x84\x36\xe1\xb5\x8a\x66\x80"
	"\x92\xdd\xad\xb7\x00\x53\x13\xc1\x01\xe3\x79\xbf\xc1\xdd\x2a\x76\x8d\x32\x6a\x18\xff\x94\xe1\xd9\xdd\x11\x58\x02\xff\xd5\xa0\xa4"
	"\x90\x77\x6b\xe6\xa8\xb4\xc4\x0a\x9b\x38\x49\xa8\xc2\x0f\x1a\x26\xd9\xaf\x71\x6f\x11\x21\x49\xd5\x9c\x80\x52\xa0\xd3\x5e\x26\x1b"
	"\xa1\xf6\x03\xa0\x19\xf8\x5f\x6e\x78\xab\x75\x82\x05\x72\xfc\x90\x63\x21\x86\x28\xc5\x99\x54\xaa\xf8\x90\x72\x1b\x48\x1b\x05\x87"
	"\xc7\xf4\xd9\x04\xa9\xeb\x37\x85\xdf\x05\xd6\x3c\x6c\x6f\x54\x43\xa9\x58\xc8\x90\x3e\x79\xa4\x78\x7c\x05\xce\x0e\xe4\xd0\x14\xe8"
	"\x60\x34\x39\x7c\x3a\x1e\xca\x78\x4b\xcf\xfe\x09\x05\xc7\xe3\x32\x6c\xd8\x9a\x2a\x91\x90\x40\xea\x67\x11\xdc\xc5\x1d\xf3\xb5\xa6"
	"\x63\xbc\xe5\x64\x5d\x4c\xe1\x20\x61\x3b\xa5\x15\xbc\xe7\x87\x69\x16\x29\x72\x94\xa3\xfd\x7c\x10\x14\xa4\x44\x2e\xa8\xdd\x98\x49"
	"\xf3\x93\xac\x73\xdf\x87\x30\x7a\x61\x61\x54\x79\x24\xd7\x6d\xcf\x4c\x7d\x97\x48\xf8\x4b\xac\xfe\x8a\xd9\x6b\xd4\x02\x02\xab\x15"
	"\x71\x50\x6e\x7c\x94\xf1\x0a\x66\xfb\xe7\x24\x47\xc0\x52\xff\x30\x09\x38\x63\xb2\x8c\xea\xae\x75\x9c\x02\x94\x1e\xe4\xd8\x09\x1e"
	"\x1e\xdf\x0e\xca\x91\xcc\x11\x54\x77\xe0\xd5\x56\x1a\x71\x15\xaa\x0f\x2f\x32\x91\xd4\x47\xc2\x51\x20\x6b\x12\x56\xa7\x2d\x16\x0e"
	"\x2a\xae\xa4\x8e\xba\x6c\x2b\xa8\x71\x9e\x69\xbe\x21\xbc\x42\x03\x21\xb4\x0c\x83\x3c"
;