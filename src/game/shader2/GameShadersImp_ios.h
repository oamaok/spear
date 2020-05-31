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
	{ { 1 }, {  }, { 1,2,3,4 }, 0, 675 },
	{ { 2 }, { 1,2,3,4 }, {  }, 675, 3804 },
	{ { 2 }, { 1,2,3,4 }, {  }, 4479, 3804 },
	{ { 2 }, { 5,2,3,4 }, {  }, 8283, 4098 },
	{ { 2 }, { 5,2,3,4 }, {  }, 12381, 4098 },
	{ { 3,4 }, {  }, { 1,2,3,4,5,6 }, 16479, 1636 },
	{ { 2 }, { 1 }, {  }, 18115, 3598 },
	{ { 2 }, { 1 }, {  }, 21713, 3598 },
	{ { 2 }, { 5 }, {  }, 25311, 3892 },
	{ { 2 }, { 5 }, {  }, 29203, 3892 },
};

const SpUniformBlockInfo spUniformBlocks[] = {
	{ }, // Null uniform block
	{ "Transform", 128 },
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
};
const SpAttribInfo spAttribs[] = {
	{ }, // Null attrib
	{ "a_position", 0 },
	{ "a_normal", 1 },
	{ "a_tangent", 2 },
	{ "a_uv", 3 },
	{ "a_indices", 4 },
	{ "a_weights", 5 },
};

const char spShaderData[] = 
	"\x28\xb5\x2f\xfd\x60\x47\x80\xe5\x3d\x00\xba\x42\x90\x0c\x2b\xd0\x6c\x8c\x0e\xd8\x8a\xb4\x20\x32\xd5\x1e\x58\x3a\xa4\x70\xcb\x98"
	"\xd1\x67\x31\x0b\x2d\x4c\x78\x10\x3a\xd3\xc4\x68\x4b\x32\x9c\x5a\x11\xd8\xf1\x00\x5f\xcc\x98\x0c\x00\x03\xb8\x00\xbf\x00\xbe\x00"
	"\x10\x2b\xbb\xd7\x13\x5b\x8b\xa5\xbb\x46\x24\xb0\x69\x45\x12\x9b\x56\x2b\xc0\x15\xa5\xd8\x79\x87\x8c\x93\x23\xaa\x6a\x6e\x90\xc1"
	"\xd5\x8c\xc0\xee\x88\x32\xb5\x97\x35\xb1\x8c\x2d\xae\x4c\xf0\xa5\x5b\x31\x18\x00\xce\x04\x26\x7b\x0d\xc3\x70\xae\xfc\x79\xa6\x52"
	"\x36\x9a\x0d\x51\x57\x50\x57\x38\x93\x73\x0e\x42\xf1\xaf\xb5\x3f\x77\xad\x45\x26\xf6\xf2\xff\xd4\x5d\xbb\x75\x5a\xaf\x2a\xc6\x38"
	"\xce\x34\xf1\x30\xe9\x4b\x7a\x5a\x0a\xa7\x02\x76\x67\x7e\xb8\x95\xb1\xa5\xde\xec\x0e\x4a\xb1\xd2\xd8\xe5\xd6\xea\x56\x0a\xe6\x52"
	"\xaa\x14\x8c\xce\xf0\x84\xc9\x26\xf3\x32\xd2\x97\x5e\xee\xd4\x93\x79\x49\xda\x69\x65\x7c\xe3\x54\x31\x7c\x4d\xe6\xa3\x0a\xc5\xfc"
	"\xd6\x3b\x48\x5d\x5c\xf5\xac\x60\xdb\x74\x86\x35\xd3\x3a\x0e\x6c\x48\xbf\xb8\x62\x99\x62\x02\x04\x31\x4e\x10\xf0\x8b\x53\x87\x53"
	"\xcf\xea\x12\xc5\x26\xe5\xfa\x56\x49\x43\x24\x42\xf5\xcc\xec\xc4\xd0\x0b\x60\x60\x90\x48\xa8\xff\xfd\x59\x2c\xb5\xca\xd3\x66\x15"
	"\x7a\x61\x13\x06\x07\x84\x05\x06\x04\x3c\xc1\x11\x2c\x34\xa2\xc6\x00\xb9\xd0\x88\x52\x84\xa2\x07\x42\xd2\xc2\x97\x56\x0b\xea\x8b"
	"\x33\xfe\x70\x40\x20\xd4\x21\xfb\x72\x59\xfc\xcf\x9b\xce\x14\xb0\x3f\xa7\x2e\xd9\x97\x8b\x9c\x11\x97\x9d\x0f\xc9\x1f\x44\xe3\xaa"
	"\xa1\x45\x68\xd0\x60\xe2\x70\xdc\x7e\xa4\x0d\x2d\x67\xdc\x45\x8b\x43\xcb\x6f\xce\x34\xcd\x52\xda\x7f\xbe\x4f\x5d\x6e\x08\xc1\x3f"
	"\xc6\x21\x14\xdb\x65\x4d\x5b\x39\xf5\xed\xb8\x87\xc6\x95\xe4\x0f\x5a\x9a\x44\xa5\xda\x88\x8a\xc3\x84\x01\x42\x1a\x30\x61\xea\xe9"
	"\xd0\xb7\xe2\xeb\x52\x34\x33\xb5\x66\x45\x46\x58\xcd\x6b\x32\xb9\xb0\x89\x01\xd3\x11\x97\x1a\x97\x52\xd3\xb7\x5e\x4a\x5d\xdb\x0c"
	"\xa3\xd9\x49\x31\x12\x28\xd0\x58\x58\x90\x6f\xbf\xbd\x31\x5f\x60\xae\x69\xd1\x8b\xf5\x25\xb7\xd1\x40\xcf\x6d\x08\xde\x49\x90\x7c"
	"\xbb\x65\xf0\xf5\x34\x1a\x10\x92\x0b\x90\x8c\xae\x00\x8c\x62\x04\x21\x75\xe0\x7b\x6f\xca\x25\x3b\xe0\x87\x3c\xe9\xad\x76\x02\x56"
	"\x38\xc5\x2c\xfd\xc4\x83\x96\xe7\x51\x38\x8f\x87\x44\x02\xf9\x6b\x50\xb8\xc7\x9f\xc4\x49\xbe\x38\xa4\xd1\x4d\xa5\xfe\x98\x18\x77"
	"\x8b\x47\x08\xb6\x99\x5d\xaa\xf5\xdc\x90\xd3\x6c\x36\xe4\xda\x5f\xab\x30\x84\xe6\x4c\x6f\xaa\x4f\x7d\xf1\x54\x19\x63\x06\x9b\x3e"
	"\x44\xb3\x2e\x90\x36\x73\x10\x19\x28\x9c\x03\xbf\xa5\x2a\x9c\xe9\xa1\x4e\xc1\x21\x3b\x93\x13\xe0\x5e\xd3\x98\x82\x83\xc6\x6a\xe4"
	"\x0c\xea\x53\x5b\x5a\x65\xb0\xac\x56\x13\x42\xbd\xd4\xfc\xc0\x3a\x13\x44\x14\x49\x19\xae\xd1\x80\xa0\xbe\xd7\x25\x2f\x57\xeb\xcb"
	"\xd5\xd2\xca\xf7\x42\x2b\x5f\x27\xe3\x73\x4f\xa1\x70\x2e\x0c\x5f\x99\x4c\x66\xb3\x81\x52\xc6\xa8\xd1\x2c\xa4\x8c\x71\x43\xcf\x37"
	"\xb4\x6b\x06\x67\xea\xce\x14\xb5\x5a\x15\x0e\x84\x5c\x88\x13\x28\x97\x30\x7a\xf1\x82\xab\x5a\x06\xf8\xa1\xb2\x19\x67\x6e\x95\x70"
	"\xa0\x99\x06\x46\x2b\x19\x8d\xfe\x8b\xd8\x1f\xc4\x62\x74\xd0\xfe\x6d\x36\x74\x41\x7b\xc6\x35\x16\xda\x76\xa3\x25\xa0\xb1\x2d\x63"
	"\x6c\x2d\xd0\x16\xd0\xd8\xcb\x41\x33\x73\x56\xe0\x90\x19\x8d\xa3\xae\x70\xfe\x24\x2f\x42\x2d\x9c\x74\x6a\x71\x60\x82\xa7\xde\x9e"
	"\xf1\xf3\x64\xa8\xbf\xe7\xa2\xb6\x2b\xb4\x6c\x88\x62\x5c\xf2\x5c\xc2\x73\x0d\xb4\x80\x1a\xa8\x47\x24\xb0\xe9\x40\x4f\x6b\xd3\x14"
	"\xe8\x69\xb9\x47\x5d\x42\x93\x7f\xae\xd9\x90\xc3\x64\xad\x35\xe8\x69\x05\x82\x04\xa8\x32\x0b\x59\x2a\x45\x33\x23\x01\x80\x80\x04"
	"\xa2\x09\x22\x90\xa7\x71\x98\xf6\x78\x1c\x12\x60\xc2\x30\x0d\x82\x24\x48\x29\x85\x0c\x31\xc6\x08\x00\x08\x00\x04\x10\x40\x80\x80"
	"\x00\x20\x02\x00\x04\xb2\x71\x73\x07\x9b\xe1\xe8\xe3\xbf\xa5\x50\xbe\xf6\x05\x47\x78\xd7\x02\xef\x8e\xbc\x6b\xef\xcc\x39\x41\x4f"
	"\x62\xd8\xb6\x8e\xa2\x56\x8c\x8d\x5f\x91\x68\xb9\x48\xb4\x19\x68\xb2\xa5\x7f\xab\xb5\x11\xe2\xbc\xbf\x10\x6d\xb4\x0d\xa1\xfd\x1f"
	"\xe2\x29\x7c\x17\xa8\x87\xd0\x3e\x46\xc4\xd6\x6d\x66\x5d\x4d\x50\x7d\xa0\x77\x12\x37\x8c\xdf\xeb\x31\x04\xd5\xd0\xa1\xef\xf1\x06"
	"\x3c\x2d\x68\x46\x96\xd8\x34\x69\x28\x59\x64\x9c\x17\xc3\xc5\x70\x5b\x9a\xcf\xd0\x2c\x33\x39\x0b\x6d\xe7\xfd\x6b\xfb\xbf\x65\x00"
	"\x18\x08\x42\xa0\xd1\xcf\xcb\x43\xad\x02\x23\xa3\xca\x00\x46\x53\xcf\xeb\x92\x0e\x11\xe4\xee\xc8\x89\x17\x3d\x99\x84\xc7\x03\xea"
	"\xa4\x93\xda\xbb\xfc\x16\x94\x7b\xa3\x26\x03\x01\x27\x16\xba\x37\x25\x73\x56\xce\xa0\x3f\xa8\xd0\xdd\x01\xc1\x52\x9d\xb8\x72\xd1"
	"\x1e\xc5\x71\x27\x79\x51\x92\x03\x4d\x8e\x41\xfa\x64\x47\x93\xe0\xd3\x06\x42\x39\xde\x5d\xd9\x64\x81\x56\xa1\x76\x05\x14\xe6\x38"
	"\x33\x80\x9a\xa4\xfe\x4d\x2a\x6e\xdd\xff\x55\xab\x8a\x01\xc5\xcd\x58\xe7\xce\xc4\x02\x08\x5b\xac\x0e\xf7\xb0\x3d\x02\x62\x94\x30"
	"\xe1\x30\x1d\x06\x50\x5b\x92\xe9\xa0\x12\xa3\xde\x12\xcc\x4c\xe8\x70\x71\xaf\xa6\x6e\xa8\xea\x08\xd9\xe8\x9a\x75\xe7\x01\x8a\xe1"
	"\x2c\xa3\x3b\x9a\xfd\x3c\xfc\xd8\xe5\xfb\x53\x8b\x89\x6a\xa2\xb5\x62\x15\x3e\x05\x3f\xc4\x35\x7f\xc4\xdf\x62\xe4\x41\x38\xab\xbb"
	"\x24\x6b\x83\xb0\x73\xa3\x7f\x2c\x00\xb9\x75\xad\xeb\x74\xc7\xc3\x6f\xaf\xb4\x55\x05\xfc\xee\x76\x1f\xf1\xe0\x59\x84\xc4\x65\xc5"
	"\x2d\x52\x70\x85\x2b\x30\x2e\x05\x29\x25\xb0\xbb\x0b\x1e\x60\xa3\xee\xa9\x05\x13\xd9\xe2\x37\x2e\x73\xf0\x7a\xd2\x47\x8a\x75\xd5"
	"\xce\x98\x7e\x6d\xea\x9e\x06\x1a\x41\xe0\x2b\xee\x9a\xaa\xba\x53\x50\x27\x5b\xf7\x4d\x82\xa2\xff\x97\x1a\xc2\x96\xb5\x4a\x83\x6b"
	"\x19\x34\x62\x54\x37\x8a\x62\xef\x1f\x6b\x29\xa0\xd7\x7d\xb3\xb3\x52\x7a\x30\x22\xe1\x86\x94\xc9\x05\xdf\x04\x0c\x46\xc7\x4a\x43"
	"\x7b\x52\x26\xfc\x14\x95\x29\x58\xd4\x8d\x2d\x15\xc1\x90\x6c\x2e\xe8\x9e\x89\xb0\x00\xbe\x14\x8a\x2e\x26\x20\xa8\x2a\x12\x68\x93"
	"\x44\x7a\xf0\x67\x03\x14\x4f\x80\xa4\x22\x97\xb3\xc2\x30\x1c\xf8\x22\xf9\xd1\x1f\xce\x0c\xbb\x8b\x90\xf5\x96\x1f\x93\xc3\xe1\xab"
	"\x7e\xb7\x91\x4b\xe9\xe9\x80\x15\x90\xdf\x79\x38\xde\x06\xcb\x17\x43\x58\x18\x2a\xc0\x3a\x9e\x90\x1b\x8c\x7f\x8b\x2a\x2a\x65\xec"
	"\x63\xa5\x3b\xd2\x40\x76\x99\x1c\x83\xea\x34\x94\xc3\xb8\x13\xe1\x52\xf8\x25\x15\x31\x2d\x20\x22\xf7\x05\x59\xa8\x8d\x20\xb2\xdc"
	"\x80\x06\x52\x3a\x3e\xc0\xa9\x6e\x87\x80\x72\xb0\x1e\xc0\xd1\x7d\x43\x0e\xa1\x92\xe5\xd8\x26\xbb\x56\xb6\x81\x13\x54\xac\x45\x46"
	"\xb9\xfd\x73\x3f\xe6\xa1\xe2\x97\xab\x22\x43\x35\xa4\x2a\xc8\x46\x4f\x09\x93\xdb\x32\xbe\x05\x3f\x03\x02\x19\x10\x81\x13\x7c\x26"
	"\xc3\xb8\x17\x65\x0b\x16\x1c\x95\x21\xe9\x6c\x56\x12\xbd\xf6\x2e\x4d\x5a\xe7\x6e\xb3\x84\x15\x93\x8f\xaa\xbd\x55\xa8\x51\x2f\x14"
	"\x53\x45\x04\x17\xbd\x18\xa2\x60\xdf\x2f\x3b\x6b\xff\xee\x5a\x40\xfe\x76\x78\xf8\x95\xc9\x53\xaa\xe4\x44\xfb\x1f\x3c\x79\x63\xd0"
	"\x7d\x96\xb5\x40\x51\xcb\x29\x6f\xe1\xb6\xe1\x7a\xdb\x08\x9c\xd7\xd0\xd0\xaf\xf8\x20\x00\xe9\x00\xe0\xfb\xe1\x6f\xc5\x47\x86\x98"
	"\xd8\x23\xcf\xc8\xd1\xd6\xed\x16\x24\x31\x1f\x18\x66\x78\x71\x52\x7b\x10\x65\xc8\x43\xda\x5b\xfa\xb3\xa4\x4c\xa2\x8f\xdb\x79\x58"
	"\xcf\x04\xc5\x74\xe3\x52\x37\x5c\xd3\xb2\x3c\xa8\x77\x7c\x5d\x99\xc6\x4f\x43\x24\xf9\x88\x0b\xd7\xc0\xa2\x10\xdd\xdd\x85\x42\x50"
	"\x0e\x48\x0e\xca\x01\x2c\x4b\x50\x06\x30\x53\x28\xc1\x86\xbc\xff\x91\xa7\xc5\xd6\x29\x99\xf9\xfc\x98\x7e\x44\x90\x37\x6d\x0b\xe7"
	"\x2b\x58\x30\x40\xaf\xa2\xaa\x4f\x91\x71\xb5\xdc\x24\xfa\x6b\x6e\x30\x24\xac\xa0\xf8\xac\x96\x3e\xc2\xaa\xa3\x9c\x51\x17\x6a\xca"
	"\x53\x28\xcf\xca\xb4\x77\x8a\x88\xf5\x75\xec\xd9\x14\x87\x7d\x9b\xc3\xd3\x7a\x85\xbc\x20\x9a\x8c\x36\xe0\x3c\x1e\xc5\x87\x75\xd5"
	"\x65\x33\x7f\x9a\x89\xc2\x2c\x60\x3c\x71\x78\xcf\x37\xa3\x6e\x64\xa1\x23\xeb\xda\x85\x45\xa4\x75\xaa\x0c\x05\x7b\x1d\xfa\x34\x73"
	"\x00\xa0\x28\x54\x72\x2c\xab\x52\x88\xd7\x86\xb4\x4a\x40\xd1\xc8\xe2\xda\xdb\x08\x39\xcd\xc2\x83\xaa\xaa\xb2\x39\x23\x8c\x4f\x8e"
	"\xc8\xb9\xbc\x0a\x4c\x26\x53\xcc\x2e\x9c\x42\x77\x4a\xdf\x1e\x99\x2f\x8b\x3a\xe8\x24\x6b\x76\xc8\xb9\x0f\x12\x8d\xb5\xc3\xce\x05"
	"\x93\xef\xad\xb6\x44\x88\xb4\xff\x50\x64\x2f\x4d\x49\x27\x2c\xc2\x7d\x65\x46\x42\xbe\xde\x0c\x1c\x9d\xad\x93\x06\xa0\xed\xa0\xb3"
	"\x2c\x0f\xc0\xff\xbb\x61\x17\xa6\xdc\x82\x8e\x53\xcf\xdc\xdb\xfb\xcc\x82\xeb\x80\x86\x11\xf9\x2d\xc4\x9e\xb6\xc6\x3b\xf3\xf0\x79"
	"\x51\x33\xdb\x5d\xa1\xc7\x95\xde\x2d\x48\x0f\xdd\x2e\x8c\x1c\xd9\x61\x51\xca\x4a\xe9\xa4\xae\x95\xfe\xc3\xba\x59\xbe\x37\x94\x59"
	"\x62\x82\xac\x92\x5d\x2b\x6e\x69\xa0\x4f\x18\x90\xd9\xb6\x18\x18\x83\x79\x71\x58\xff\xe5\xdb\x53\x2e\xa5\xba\xb6\x50\xf5\xca\xa4"
	"\x4e\xe6\xa7\x89\x82\x0f"
;