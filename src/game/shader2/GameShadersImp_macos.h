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
	{ { 1 }, {  }, { 1,2,3,4 }, 0, 974 },
	{ { 2 }, { 1,2,3,4 }, {  }, 974, 6810 },
	{ { 2 }, { 1,2,3,4 }, {  }, 7784, 6810 },
	{ { 2 }, { 5,2,3,4 }, {  }, 14594, 7509 },
	{ { 2 }, { 5,2,3,4 }, {  }, 22103, 7509 },
	{ { 3,4 }, {  }, { 1,2,3,4,5,6 }, 29612, 2067 },
	{ { 2 }, { 1 }, {  }, 31679, 6411 },
	{ { 2 }, { 1 }, {  }, 38090, 6411 },
	{ { 2 }, { 5 }, {  }, 44501, 7110 },
	{ { 2 }, { 5 }, {  }, 51611, 7110 },
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
	"\x28\xb5\x2f\xfd\x60\x61\xe4\xed\x5a\x00\x6a\x63\x6c\x12\x2b\xc0\xac\xa8\x0c\xd0\x09\xc9\xd3\xe6\xa8\x22\xf2\x91\x05\x59\xc1\x8e"
	"\x60\x9d\x7b\xe6\x02\xd5\x36\x2c\x51\xcd\x4d\x15\x45\x69\xab\x93\x6d\x4a\x78\x59\x04\x41\xd8\x16\x05\xe4\x1c\x01\x1d\x01\x1b\x01"
	"\x93\xfe\x49\x31\x1d\x49\xed\xb5\xa1\xe7\xab\x2f\x29\x51\xb7\xb5\x2a\x35\x55\xf6\x82\xaa\x57\x84\xaf\xaa\xd7\x34\x5d\x1c\xe2\xc0"
	"\xbb\x05\xe5\x42\x58\x58\x7e\x6c\xb3\x2b\xd1\x68\x04\xe0\xf6\xba\x74\xdd\xf5\xbe\x14\xf8\xb5\x1a\x28\x04\xf8\xca\x6e\x9d\x29\xf7"
	"\xa9\xdb\xb9\xce\x94\x7a\xab\xa7\xe4\x8a\xa9\xaf\x16\xa5\xee\x4b\x00\x80\x46\x23\xa3\x01\xc9\x1b\xeb\x76\x57\xb0\x0a\x66\x71\xbc"
	"\x63\x1a\xe7\x7a\x53\x2f\x61\x5c\xb2\xc6\xbd\x06\x3e\xd3\x1c\xdf\x48\x4d\xbe\x2f\x35\x1d\xd3\x19\x11\x5e\x72\xdb\x4e\x26\x58\x85"
	"\x44\xf2\x34\x4e\x9f\x90\x6b\xe7\x59\x53\xcf\x50\x50\xe4\x3b\x59\x4a\xed\xfc\x7e\xc3\x74\x66\xfb\xb6\x15\x59\x3f\xc6\xfa\x13\xd6"
	"\x66\x3b\x56\x71\x77\xef\xb5\xe6\x4b\x41\x21\x31\x61\xca\xcf\x59\xc1\xa6\xca\x47\x79\x2b\x62\xcf\x96\x92\x6b\x87\x10\x0a\x0a\xf7"
	"\x32\xa5\x71\xdb\x96\x63\x12\xc9\x67\x7d\xfb\xe5\xfa\x9a\x2a\x2f\x3b\xa6\xa7\x97\x6f\xb5\x32\x1a\x10\x5c\xad\xad\x33\xa4\xff\xcc"
	"\x66\xaf\x1a\x4f\xfe\xff\x1b\x11\x1a\xd6\xd6\x13\x9e\xeb\x66\xf0\x9d\xc2\x76\xe7\x8d\x06\xe4\x2d\x55\x2f\x6e\xad\x9a\x9f\x92\x2f"
	"\xc7\x1a\x0d\x02\x16\xcd\x42\x65\x5d\xde\x20\x50\x71\x71\xd5\xf9\x84\x6b\xb7\x16\xc3\xdb\x0a\xc3\x54\xbe\x02\x02\xef\x1f\xdf\x48"
	"\xff\x6a\xaf\xd5\xcf\x8d\x2f\x6b\x62\x5b\x7c\x59\x95\x37\xc8\xea\x6e\x04\x16\x5d\x55\x0b\xfe\x04\x18\x2a\x69\x37\xd6\x33\xdb\x9d"
	"\x07\x4e\x95\x4f\xb0\xbc\x56\x7c\x42\xcf\x37\x12\x21\x21\x61\x7b\xc5\x70\xad\x58\x47\x1f\x19\x51\xe8\x41\xf2\x4d\x29\x1f\x28\x7a"
	"\x12\x82\xb2\x79\x0e\xad\x21\xf2\x8e\x77\x68\x8e\x4e\x19\xa1\x0f\x33\x22\xa3\x63\xa4\x77\x98\x26\x75\xd7\xeb\xe5\x33\xcf\x4c\x26"
	"\xb7\xa2\x3e\x41\xee\x8a\x31\xca\x5e\x7c\x72\x68\x58\x60\xe0\xe5\x6b\x9e\xe5\xd8\x66\x2c\xbc\xf4\x8e\x40\xfb\xc9\x95\x70\x72\x8e"
	"\xa7\x8f\x9c\xd0\x74\x26\xa4\xf1\x45\x68\x52\x08\x4a\x28\x36\x8e\xa5\xb3\xad\x1e\xd2\x3f\x5e\xe1\x1f\x1e\x5e\xfa\x06\x08\x88\x08"
	"\x44\x46\xa1\x3c\xde\x39\xbe\xb5\xd6\x37\x04\xfe\x8e\x97\x3e\x92\x51\x28\xb1\x5a\x5c\xb8\x3e\x44\x9e\x81\xe8\xba\x8a\x68\x0e\x10"
	"\x20\x98\x1a\x22\xef\xdc\xcc\xda\x9b\xbc\xc2\xbb\xd6\xfe\xf1\x6f\x12\xe3\xcc\xde\xf1\xd9\x4b\x1f\x5f\xf3\x9e\x51\xf8\xd7\xc0\x76"
	"\x59\x15\x53\x03\x06\x74\x5e\x27\x2f\x7d\xe6\x1e\xba\xae\x22\x9f\xf0\xcc\x9b\x7c\x42\x55\xed\xb6\xe8\xed\x82\xba\xd8\x82\x9a\x98"
	"\x1c\x1a\xce\x4d\xfc\x52\x2d\xb8\xb6\x36\x4d\xd3\xf4\x36\x5f\x9c\xab\x7b\x4d\x22\xf9\xad\xb1\xa6\x02\x05\x74\x65\x2b\x70\x8d\x12"
	"\xc9\x33\x3d\xdb\xc9\x8b\x24\x7d\x9e\x39\x9d\xf9\x9c\x88\xbe\xf7\x26\x84\x11\x42\x51\x42\xe8\x38\xcf\x13\xc9\xf8\x40\x94\x3e\x10"
	"\x7c\x1c\x08\x92\x50\xf4\xa2\x8c\xa2\x09\x45\x91\xc9\x66\x06\x71\x95\x7f\xfc\xc2\x57\xe4\x55\x2d\x9b\x75\xff\xc9\x44\x8a\xd1\x54"
	"\x1e\xbd\x34\x15\x37\x65\x53\xa9\x48\xfa\x40\xce\x7c\x32\x42\xe7\x79\x9c\x27\x82\xef\x89\xa2\x07\x1f\x9c\x2f\x46\x94\x46\xf9\x38"
	"\xd4\x81\xd3\xa2\xaf\x77\x44\x34\xc2\xe8\x99\xf4\x99\x51\x14\x45\x93\xd2\x08\x25\x9d\x91\x8c\xa2\xf3\xc4\xa8\x89\xd0\xc1\xf8\x62"
	"\x94\x8e\x74\x64\x14\x69\x8c\x34\x42\x4f\x42\xf9\x1e\xc3\x67\x59\x56\x41\xd9\x13\x9a\xc1\xae\x19\xcf\x39\xcf\x38\xc7\x3b\x14\x02"
	"\x4e\x0f\x0f\x91\x08\xc4\x33\x11\xee\xe1\x99\x48\xe4\x1f\x0f\xd1\x2c\xc6\x95\xfe\xc1\xb9\xc7\xd3\xe8\x50\xe7\x99\x90\x8c\x20\x74"
	"\x20\x7c\xd1\x84\x0f\x36\xaf\xfd\x42\xc5\xb2\xed\xbd\x66\x8d\x6b\xd8\x56\x2c\x7e\xe6\x82\x91\x35\x4c\x6b\xa4\x77\x54\x5b\xeb\x03"
	"\x99\x65\x20\xeb\x35\x10\x31\x96\x00\x1e\x60\x70\x48\x0f\x61\x62\xb7\x05\xe9\x21\x1c\xd3\xba\x86\x30\xd1\xf9\x95\x78\x05\xe9\x6b"
	"\xc7\xb6\xb2\x70\xec\x56\x75\xc1\xa5\xdc\xad\x00\xbf\x94\x32\x46\x08\x21\x14\x01\xb4\x79\xad\xe7\xab\x81\xe2\x15\x29\xb9\x77\x4a"
	"\xee\xd1\xda\x39\x3f\x6b\xe7\x08\x9f\x71\x0c\xd7\x97\xc5\x82\x81\xf1\x62\x84\xd0\x5d\x2e\xed\xa5\x47\x8c\x10\x66\x8d\xe3\xb3\x26"
	"\xbb\x0d\x94\x93\x45\x6f\x27\xb3\x0b\xa7\x4f\x80\x22\xb5\x96\xc9\x2d\x75\xab\x7c\xaf\x8b\x0f\x68\x06\x15\xde\xda\x92\xef\xc5\x44"
	"\x8c\xa0\xa4\xe2\x27\xb7\x59\x1b\xb0\x5c\x98\x4b\x06\x65\x64\x9e\x48\x46\xf1\xcd\x27\x92\xd1\xe3\xd0\x29\x23\x9c\xa0\x8c\x50\xc6"
	"\x07\x7a\xa0\x07\x7a\xa0\xfe\x0c\xe4\xcf\x3c\xee\x3c\x08\xa1\xf3\xa0\x73\x18\xad\xca\x98\xde\xd8\x33\x97\xac\x61\xa4\x7f\x1c\x3f"
	"\x83\x91\x35\x17\x8d\xa6\x97\xf8\x15\xfe\x82\x1f\x6e\xf9\x5a\x10\x3f\x17\xdc\xb6\xd6\x78\xf2\x30\xd4\x5c\x2f\xfd\x05\x7f\x81\x4b"
	"\xdd\xbb\x4e\x55\xbc\x26\x2c\x17\x49\xfa\x87\x91\x1e\xf9\xd2\x29\x23\x11\x12\xf3\xa5\x07\x03\xa4\x6f\xed\x78\xc7\x5b\xa4\x67\x1c"
	"\xa5\x90\xf3\x44\x67\x46\x51\x46\xb1\x65\xf7\x4d\xb2\x46\xfa\xc8\x03\x77\xc9\x37\x6b\x1c\xf9\xb6\x16\xf1\xf2\x92\x5e\x42\xfa\x0b"
	"\x5e\x13\x4c\xe9\x0b\x48\xc7\x48\xff\xa0\xc5\x77\xfc\x25\x6b\x1c\xd3\x22\x7a\xb5\x66\xeb\xf9\x66\xfb\x3d\x5f\x8c\x71\xbb\x65\x84"
	"\xaf\xc9\x0b\x5e\xad\x08\x79\xc5\x3d\x82\xe7\xa8\x42\xfb\x96\x2a\x44\x33\x02\x00\x00\x40\x00\x00\xa2\x09\x00\x08\xe4\x69\x16\xa4"
	"\x59\x6d\x39\x12\x00\xa2\x2c\xca\x51\x1c\xc5\x91\x18\x52\x06\x11\x03\x8c\x01\x02\x02\x00\x02\x00\x00\x08\x80\x00\x00\x41\x00\x90"
	"\x99\xbc\x4e\x4d\xa3\xa3\xf9\x6b\x6c\xc3\x1b\x5a\xb5\x98\xa5\xdd\xb3\x6d\xd1\xf9\x2c\x02\xbb\x5a\xcd\x27\x1d\xd2\xa9\x1d\x6e\x79"
	"\xfb\x8a\x5a\x66\x9e\x1f\xe1\x3e\x5c\xd0\x26\xf8\xc0\x83\x5a\x1b\x82\x71\x05\xef\xcc\xcc\xe8\x1d\xee\xb8\x49\xcb\x1d\xfa\xee\xc8"
	"\xe9\x8e\x5a\x27\xfa\xfb\x98\xda\x9a\xed\xfb\xd2\x6c\xe9\x1b\xf0\x43\x32\xf0\x9b\xf3\x95\x62\x16\x38\xe1\xab\xcd\x22\x78\x57\x2d"
	"\xed\x53\xcc\x81\xcb\xa1\x87\x34\xe5\x2e\x9a\x0a\x3b\x28\xc7\x97\x82\xce\xab\xdd\x86\x68\x83\x62\x8b\xbb\xf3\x06\x78\x99\xde\xc2"
	"\x62\xca\xd4\x52\x62\x8b\x55\x32\xd2\x54\x33\xda\x24\x5e\x57\x29\xdd\xd9\x2c\x90\xcc\x4b\x1f\xa5\x31\x7d\xa3\xf7\xa5\x07\x79\x4c"
	"\x4f\x0c\x23\x33\xb3\x3d\x24\xcf\xc6\xec\x70\xa5\x4e\xab\x20\x63\x72\xc9\xa2\x98\x59\xd1\x1a\x12\x11\xea\xde\x04\xda\xc0\x1b\xb0"
	"\xc2\x24\x39\x38\xce\x2a\x63\x88\x67\x08\x04\xcc\xb8\x59\x7b\xb9\x0a\xf4\x46\xa0\x73\x07\xea\xf5\xe5\xe2\xcf\xcf\xf9\x40\x45\xbb"
	"\xc1\x7d\x41\x16\x83\xe3\x5d\x16\x68\xaf\x73\x9e\xb9\x23\x41\x53\xe5\x92\x02\x81\x03\x2d\x20\x69\xb6\x19\x49\x6e\xc2\xbf\x45\x6a"
	"\x62\x43\x0a\x54\x94\x46\x83\xe7\xd3\xf2\xdd\xf3\x03\x1c\xe2\xf0\x34\x5f\xe5\xf0\xfd\x68\xe1\xb1\xe4\xd6\x24\x24\x56\x57\x3d\x0d"
	"\xf6\xc4\xd0\x04\xad\xa5\x2e\x8b\x30\x07\x92\xb9\x5a\x0b\xe2\xdf\xdb\x90\x5b\xa5\xda\x2d\x11\x82\x71\xe9\xa9\xdb\x4e\x2e\x0e\x96"
	"\xe3\xdd\x3b\x7f\x5b\x31\xfd\x73\x1d\xe3\x11\xe2\x36\x18\x75\x30\xb3\x44\x4e\xfd\x15\xe4\x10\xba\x43\x8f\xfa\x47\x2f\x51\x7f\x50"
	"\x68\x31\x23\x79\xb8\x28\xd4\x6c\x39\x8f\xb9\x43\xfe\x7e\xfa\xe9\x2a\x36\xfe\x08\xa9\x52\x2f\x50\x01\xc5\xcd\x25\xb3\xad\xfa\x15"
	"\x4d\x0b\xfb\x17\x76\xbf\xc3\x5a\xfa\x14\xcb\x39\xd2\x19\xda\x2d\x14\x2e\x8a\xe0\x6d\x58\x67\x47\x9a\x3d\x39\x18\xfd\xb9\x8b\x0e"
	"\xb6\xe7\x1a\x27\xea\xf3\x8d\xcb\xc4\x06\xd2\xfc\x19\x88\x07\x04\x8d\x54\xce\x74\x94\x34\x00\x35\x20\xc2\x2f\x8d\xd8\xfd\xc0\xb8"
	"\x76\x7a\xa7\x5c\x21\x85\x10\x6b\x78\xec\x48\x1a\x0c\xe4\x4b\x6f\x9c\x62\x64\x46\x33\x4d\x24\x8d\x69\xcf\xb6\x87\x8f\x0e\x4f\xa5"
	"\xaa\x05\xf9\x17\x77\x5d\xa7\x84\x17\xf7\x27\x33\xe9\x06\xfe\x08\x46\x01\x14\x5a\x91\xfa\x7d\x3e\x0e\x28\xd5\x0b\x72\x16\x8b\x49"
	"\x66\xfc\x75\x06\xeb\x54\x53\xb7\xee\xf9\x14\x42\xbc\x4d\x80\xe4\x45\xaa\x19\xe6\x6e\xc8\xb8\xb4\xe8\xca\x94\xf5\x08\xa2\x24\x05"
	"\xa6\xbc\x60\x2f\x08\xd4\xc4\x0c\x14\x96\x3e\x70\xa2\x95\x34\x6b\xe5\x6a\xa5\x45\x93\x15\x0d\x8a\xcd\xf1\xef\x44\xf1\xed\x00\xaf"
	"\x94\xd6\x97\x4a\x7c\x15\xda\xdf\xbb\x39\xc4\x2e\xd2\x8b\x0d\x7e\x9f\x30\x49\x5e\x51\x8f\x2d\x53\x11\x00\x59\x7c\xe3\x2b\xec\x7d"
	"\xe0\x87\xa4\x13\x63\x8f\xa5\x87\xae\xf5\x6d\x9e\x19\xb7\xfe\xc9\xe9\xce\x83\xdb\x64\x4e\x52\xbb\xae\xe1\x41\xaf\x6a\x76\xee\x5c"
	"\x90\xd3\x6c\x5f\xac\x09\xb9\x7b\x09\xd6\x35\x67\x07\x85\x26\xb2\xd9\xc3\x35\xa2\x42\x28\x3e\x5b\x32\x95\x31\x11\x34\x96\xef\x61"
	"\x43\xd7\xa1\x6d\x0c\x49\x9f\xd0\xd0\x87\x6f\x06\x0b\x3a\x6c\xd3\x80\x70\x88\xac\xca\x3c\xa9\x87\x99\xf7\x1b\x9c\x24\xc9\x46\xd4"
	"\x16\x97\x4d\x51\x46\x57\xd7\x54\x36\x6a\x8a\x1a\xf2\xec\x48\x0b\xba\x21\x23\x68\x14\x64\xda\xf8\x31\x0c\x1d\xc9\x78\x22\x3b\x2b"
	"\x59\x5a\x8b\xe7\x6f\x87\x09\xac\x5d\x04\x0c\xd6\x8f\x8c\x98\xd0\x50\x82\x1c\x89\x76\xde\xc0\x28\x8b\x67\x5a\xca\x0f\x53\x4a\xe8"
	"\x8c\xe0\xc2\xfa\x39\x57\x9c\x86\x96\x82\x3f\x64\xb0\xda\xe3\x12\xeb\xdf\x4c\x96\x94\xa8\x5a\xff\xab\x0c\x04\x73\xc9\x8e\x81\x92"
	"\x13\xce\x15\x9b\x9d\x93\x02\x5a\x41\x61\x03\x88\x05\x1b\x09\xd9\x53\xd3\x8d\x0a\xef\xa5\xa4\xb4\x20\xd5\x2b\xd8\x91\x29\x83\xdc"
	"\x9d\x89\x25\xe0\xd5\x95\xb6\x4e\xf0\x9a\xcf\x46\xfa\x00\x1d\xe4\x4a\x98\xf0\xa3\x2c\xa8\xd5\x34\x5c\xcb\xca\x94\x60\x03\xa4\x3c"
	"\x6a\xa9\xc8\x25\x44\xd8\x40\xb5\x77\x8b\x20\x35\x0b\x75\x8f\xd9\x0c\x2a\x82\x5b\x6a\x83\xd0\x27\x4a\x52\x58\x87\x0b\xff\x04\x3a"
	"\x37\xd6\xaa\x23\x2a\x09\x66\xce\x7a\x48\x74\x53\x2d\x44\xc9\xc6\x08\x44\x01\xe1\xd6\x7f\x27\xb5\x2d\xde\xe0\x58\xc2\x27\x42\x59"
	"\x2c\x8f\x6b\x17\xa8\xb0\x0e\x2d\xbc\xdd\x5c\x2c\xeb\x38\xeb\xec\x33\xe3\xec\xfa\xd8\xb9\xb2\xb5\x7c\x01\x01\x28\xca\x37\xf4\xf5"
	"\x01\xc4\xab\xc6\x5a\x27\xe0\x7b\x03\xb3\xde\xfa\xc1\xa6\x68\x47\xd5\x87\x2a\x68\xad\x4c\x24\x9c\x43\x4f\x7f\x38\x94\x53\xae\x17"
	"\x06\x76\x2e\x92\xa3\xb8\x39\xfa\x92\xa5\x92\xae\x4e\x36\x58\x0f\xf8\x35\x5e\x98\xf8\xc7\xeb\xdb\xc1\x69\xfd\x4c\x91\x15\x41\x7e"
	"\x9f\x6f\xc0\x70\x1c\xa1\xb1\x6a\x5d\xdc\x85\x0a\xac\x49\x0a\x51\x9a\x6a\x1a\xe7\x73\x8c\xf6\x4a\xe6\x0c\x2d\xcf\x7b\xf6\xe3\xf6"
	"\x0e\x19\x26\x6a\x54\x67\x87\x8b\xda\xbb\x91\x1b\x0c\x61\xbd\x08\x6f\x3c\xa5\xef\xd0\x01\xb7\x4b\x42\x73\xd8\xc6\x57\xf2\xcd\x73"
	"\x1a\xd8\x2a\xac\xbd\x44\x64\x26\xa1\x98\x55\xe9\xfc\x48\x5b\x3e\x0d\x2b\x5f\x55\xb4\xee\x62\x5f\x84\xae\x84\xf4\x55\xaf\x03\x1c"
	"\xbb\xee\x8b\xab\xa4\xf7\xbc\xda\x29\x53\xef\x04\xe4\xd4\xc5\x26\xb6\xe1\xdd\x6b\xa4\x99\x42\x55\xfb\x1d\xa7\x49\x09\x03\xeb\x9b"
	"\xd8\xb0\x6d\x13\x20\xcb\x8e\x70\xce\x11\xd0\xae\x9a\x86\xd1\x36\xba\xbb\x11\xd6\x16\x9d\x0d\x19\xd6\xa5\xb8\x76\x15\xd2\x83\x1c"
	"\x78\x07\x83\x34\xce\x99\xc7\xb0\x75\xe9\x51\xb8\x50\x86\x8a\x5b\xf4\x34\x15\x9f\x55\xb9\xe3\xd8\x18\x4c\xd4\x99\xe8\xaf\xb5\xe8"
	"\xc6\x4b\xe2\x35\x08\x61\xec\x7e\x2f\x49\xfc\xd2\x5a\x0e\x80\x2a\x68\x35\x52\x5b\x15\xec\xda\x59\x3b\x7f\xb1\xe3\x18\xa9\x8a\xae"
	"\x6d\xd2\x72\x98\xc8\xdf\xcf\x0d\x3a\xde\xdd\x94\x6c\xcc\xdd\x3c\xc7\xc4\x85\xb0\x66\xb7\x76\x71\x22\xd3\x38\x1f\x60\x88\x24\x7b"
	"\xa3\x1e\xb8\x1b\xd6\xaf\x0f\x8a\x8a\x90\x23\x09\x41\x95\x4e\xc8\x0e\xf4\xbe\x71\xf2\x59\xb2\x4a\xd2\x87\xc8\x17\x15\x3d\x2f\x5e"
	"\xf0\xcd\xd8\x04\xea\xad\x0b\x1c\x11\x70\xc2\x57\xd8\x1c\xa1\x8f\xc5\x31\xff\x4b\x15\xad\x34\xa5\xea\xc3\x74\x0d\x80\xe1\x80\xef"
	"\xc5\x13\xd4\xa7\xf8\x0f\x03\xbb\xcc\x3c\xc3\xe7\x8f\x5d\x23\x97\xd2\xd0\xa8\x00\x28\x25\xdd\x70\x52\xdb\x6d\xfb\x7d\x6d\x4e\x14"
	"\x78\x74\xb5\x64\x85\x9a\xbe\x5f\xe7\x17\x8c\xc2\x91\x1a\xca\xa1\x11\x3c\xa7\x60\x13\x3b\x9b\x42\xe3\xa8\x86\x53\x76\x3d\x10\x79"
	"\x22\x43\x79\x57\xd8\xed\x95\x57\xd6\x80\xe2\x68\x6f\x15\x07\xf4\xec\x2b\x70\xff\x70\xf0\x43\xb7\x08\x7d\x21\xc4\x64\x71\x9b\xad"
	"\x8a\xbc\x46\xcc\x41\x9e\x6f\x86\x62\xd0\xf6\xcd\x31\x95\x35\xb9\xe4\x1b\xae\x64\xcd\xfb\xc7\x52\x1a\xdf\xb8\x08\x5e\x08\x40\x66"
	"\x5e\xbb\x82\x68\x3e\x05\x3d\x26\x09\xa7\x42\x76\x4b\x98\xb7\x0d\xf3\xad\x16\xdb\xb3\x5f\x44\x4f\x13\xe5\x8b\x08\xb5\x3f\xf8\x15"
	"\x48\xf4\x40\x18\x37\x60\xff\xc0\xe0\xe5\xe0\x11\x92\xb1\x1f\xc6\x96\x0a\x2b\x00\xd8\xf7\xf8\x30\x0b\x3c\xaf\x57\xbf\x17\x1c\xe6"
	"\x08\x25\x7f\xdc\xa6\x90\x6c\x0f\x67\x49\x9c\x65\x9d\xc8\x5d\x74\x6f\x9c\x0a\x56\xe7\xc1\x25\xcc\x9f\x86\xff\xe5\x80\xa0\x10\x42"
	"\x19\xea\x07\x36\xcd\x23\x8f\xfd\x3b\x26\xc5\x00\xd9\xb4\x91\xf9\x3f\x69\x28\xe0\x7b\x1d\x27\xd0\xfd\x13\x21\xae\xe0\x07\xee\x47"
	"\x67\x00\x09\x30\x8b\x1c\x85\xd9\xd4\xa4\x01\x6a\x7f\xef\x8f\x1a\xa4\xfc\x54\x0c\xf5\xa7\x8b\x23\xff\x99\xd6\x2b\x51\xb6\x10\x3f"
	"\xa5\x94\x96\x30\x36\x08\x56\x12\x34\x68\x4a\x61\x03\x85\xfe\xa1\x28\x52\xcb\x3a\x46\x43\x2f\x4b\xef\xbc\xa0\x01\xc9\x16\x7d\xf6"
	"\xbe\x11\xac\xc2\x16\xc0\xf6"
;