#include "SpLicense.h"

#include "ext/sp_tools_common.h"
#include "sp/Json.h"

namespace sp {

// Generated by misc/gen-licenses.py sp-licenses.txt
static const uint32_t uncompressedSize = 10101;
static const char compressedLicenses[] =
	"\x28\xb5\x2f\xfd\x60\x75\x26\xbd\x47\x00\x7a\x54\x58\x0f\x2c\xc0\x8e\xaa\x03\x08\xd3\x98\xb2\x64\xd3\x9b\x4e\x19\xda\xb9\xbc\x00"
	"\x91\x2a\x53\x57\x60\x0f\x2a\x85\x34\xab\x2f\xdc\xa0\xc2\xb5\xc9\x11\x14\x81\x08\x00\x00\x00\x00\x00\x14\x0f\xe0\x00\xf7\x00\xe6"
	"\x00\x55\x66\xcc\x59\xcc\xad\x50\x5b\xa9\xd0\xce\x97\x6e\x95\x9e\xb5\x66\x6a\xbb\x33\xf6\x8c\xad\xd4\x5e\x29\x59\xb2\x33\xca\xca"
	"\x37\x6e\xaf\xf4\x95\x3a\xdc\xe6\xad\xcb\xa8\x76\xce\x3a\xad\x65\xa3\xf2\x26\xe3\xcc\xb4\x9e\xbf\x95\x0d\x67\xf2\xd2\xae\xa0\x58"
	"\x6c\x19\x2f\xd7\x5f\xdb\x69\x34\xab\xd0\x0b\x78\x82\xcb\xdb\xad\xd4\x9a\x66\xb7\xb4\xc4\xd0\xca\xf9\x6a\xb2\x82\x56\xee\x1a\xdf"
	"\x2b\xb7\xca\xdc\xf9\x6a\x62\x26\x2b\x52\xa3\xdb\x6f\x47\x2b\xfd\xe5\x4a\xcc\x1b\xbb\x55\x5a\x79\xa6\xc6\x96\x18\x67\xb5\xa3\xf6"
	"\x2d\xff\x37\x95\xb7\xc3\x64\xbd\x56\x6a\x4c\xf5\xd5\x57\x43\xe3\x8a\x7a\xfb\x39\x9d\x37\xb2\x86\x6f\x28\x28\xc5\x38\x8b\x8f\x37"
	"\xb7\xd9\x72\xe5\x18\x06\x06\x3c\x40\xbc\x21\x63\xf8\xbd\x58\x64\xcd\x70\xf4\xb9\x14\x38\xc2\xe2\xf2\x9c\xf4\x79\x77\x9c\x58\x40"
	"\xb0\x14\x1d\x2f\x6c\xbb\x38\x1f\x06\x81\x43\x95\x83\xcb\xdc\xc5\x9b\x85\x56\xa3\xd6\xf1\xda\x07\x8b\x27\x58\xf0\x16\x7b\x61\x01"
	"\x42\x11\x90\xf3\x07\x7c\x03\x29\x8e\x44\x05\x1b\x5e\x05\x25\xa6\xf7\x48\x0f\x02\x23\xc8\x93\x6e\x59\x4f\xe5\x50\x2f\x22\xf1\x66"
	"\x82\x9a\xa4\x09\x72\x24\x1a\xa4\x09\x5a\x13\x0a\x45\x3d\x0b\x56\x0a\x2a\xc7\x6a\x5e\x45\xa9\xa6\xa8\x2f\x05\xd5\x7b\x60\x04\xf5"
	"\x2a\x68\x9a\x20\x7f\x16\xc5\x82\x56\xe5\xa0\x54\x92\x47\x51\x1e\x86\xbe\xaa\x07\x71\x24\x0e\x74\x4a\x83\x57\x35\x13\x94\xf3\xe0"
	"\xf3\x26\xd5\x9b\xe8\xb1\x9e\x47\x41\xe8\x45\x0e\xe7\x39\xa2\xc8\x89\x54\xb7\xa4\x17\xf1\xde\x80\x9e\x04\x3a\xd1\x53\x41\xa8\xaf"
	"\xbf\x6a\x9a\x50\x94\xc9\x29\x27\xa2\x3c\xd0\xa2\x9c\xc9\x9d\x87\x64\xc1\xc7\x84\x05\x23\xe8\x70\x22\x0f\xf5\x24\x7f\x4c\xef\xab"
	"\x99\x2c\x0b\x4a\x60\x74\x6b\x92\xfe\x06\x22\x41\x88\x12\x3d\x12\x7c\x53\x3d\x68\x49\x0e\x05\xa3\x89\x04\x9e\x9a\x5e\x55\x8f\x9a"
	"\x38\x78\x9f\x90\x9c\xbe\x16\x57\xe5\x2c\xae\xc9\xa2\x38\x20\x65\x49\x90\xe4\xbc\xa2\x1e\x14\x4d\x91\x5b\x11\x08\xa4\x26\x26\xd3"
	"\xf3\x17\xb1\x3e\xe1\x38\xf4\x20\x18\x31\x51\x93\x3f\xca\x91\xb0\x1e\x53\xf5\x0a\x46\x2c\x6b\x3b\x99\xd9\x17\xd7\x6b\x27\xc7\xc5"
	"\x6c\xa3\x19\x5b\xca\xd5\x8e\xe3\xa6\x8d\x5b\xc9\x96\xb8\x45\x8d\x27\x16\xb5\xc9\x17\x5b\xbb\x39\x6f\x73\x05\x1f\x66\xe4\xe4\x36"
	"\x49\x81\x9b\x00\x56\x32\x6c\x0c\x20\xe7\x2d\x74\x56\xac\x95\x2c\x71\xae\x5a\x3b\x96\xe6\x1c\x30\x39\x2b\x4c\x4a\x41\x2b\x04\x12"
	"\x8b\x62\xf8\xe5\x46\xc9\x79\x93\x77\x46\xa6\x22\xe7\x41\xff\x93\x26\xc9\x44\x3d\x9a\xc7\x69\x28\x0a\xf5\x90\xa0\x04\x49\x7d\x35"
	"\x0c\x22\x16\x7c\x95\x72\xbf\x35\x32\xf6\x39\xe3\xc5\x5a\x37\x63\x09\xd4\x9e\x6a\xdc\xb5\xab\x6c\xf4\x82\xb1\x72\xb2\x6d\x6c\x73"
	"\xc1\x3a\x8e\x9b\x86\x5e\xb9\xc9\xa9\x75\x33\x6b\xf9\x8c\x6b\xd5\xd7\x8c\xac\x35\x2c\xae\x6a\x71\x31\xdc\xf2\x94\x95\x47\x99\x51"
	"\x6b\x63\x85\xf6\x15\x31\x6a\x33\xd4\x52\xe3\x85\xb9\x56\xde\x68\x7c\x3b\x56\xcb\x33\x18\xdf\xca\x4f\x0e\xe0\x3f\xc0\xb6\x39\xe3"
	"\x7b\xf9\x92\x71\x7d\xdc\xc6\x49\x2f\x99\x94\x1a\xa2\xc7\x61\xa8\x98\x8c\xec\xb6\xc9\x76\xde\x5c\x58\x34\x22\x0f\x9c\xc0\x11\x2e"
	"\x2c\x1c\xec\x74\xb0\x54\xda\x88\x0d\x0c\x06\xe3\xe0\x90\x80\xbb\xf7\xd2\x57\x81\x87\xae\xe3\x82\x37\x11\x12\xea\x81\x2f\x59\xe4"
	"\x9c\xda\xcc\x60\x78\x76\x6d\x3c\xe3\x5c\xb5\xc9\x90\x6b\x92\xca\x9a\x0f\x64\xd8\xfb\xfa\x1b\x95\xb5\xee\x6a\x77\xbb\x4a\xe6\xca"
	"\xee\x32\x77\xb1\x2d\xb3\xf8\x8b\x59\xcb\x99\xad\x92\xb5\xbe\xe2\x95\x15\x6e\xc6\x28\x24\xde\x97\xad\xd0\xc6\x62\xe2\xac\x20\xdb"
	"\x42\xb3\xa8\x59\x59\x3b\x09\xb6\x23\x6f\x8c\xa6\xdd\xc4\xdb\xd8\xf2\x4d\xa5\x6f\x1a\x6e\x6b\x68\x1c\xdb\xc8\x93\x2b\xd6\xdc\x97"
	"\x6b\x8d\x8e\x77\xe4\x6a\xa3\xd0\x27\xf3\x0c\x9e\xc7\xae\xbd\x01\xde\x78\xce\x6b\xfc\xd0\xad\x82\xec\x20\x1b\xfc\x03\xc4\x03\x04"
	"\xa3\x40\x03\xc3\xbb\xae\xf4\xd9\x26\xd9\x59\x36\xf8\x1a\x19\xa2\x1b\x57\x02\x97\x8c\x29\x6a\xdc\x9e\x99\xcd\xd9\xca\x15\x1a\x3b"
	"\x6f\x40\x34\x64\x37\x81\x2b\x57\x6d\x76\x14\x63\xd1\x78\x62\x68\x53\x64\xf1\x15\xf8\x19\x2f\xad\x98\x8e\x66\xae\x80\xb9\xb6\xa4"
	"\x6f\x0c\xf8\xff\x82\x4a\xa8\x92\xab\x9a\x93\xcc\xcc\x00\x0c\x00\x00\x40\x00\x32\x20\x00\x04\xc3\x11\x4d\x52\xf2\xf4\x72\x01\x41"
	"\xc6\x81\x46\x19\x46\x40\x01\x01\x08\x00\x04\x00\x02\x00\x11\x09\x08\x40\x02\x08\x7d\xd8\xc9\x62\x7f\x06\x8d\x0c\x6e\x7f\xc3\x22"
	"\x6a\x42\xea\xfe\x62\x06\x92\x13\x93\x4d\xdc\x59\x29\x21\x9d\x82\xeb\xf6\x4c\xbf\xcf\x2c\x78\x86\x6c\x3f\x67\x0f\x63\xd6\xdc\xb4"
	"\x4f\x88\x0e\x6c\x0a\xcc\xc9\x19\xb0\xde\x23\xf0\x9c\xc3\x09\xf2\x61\x5a\x16\xf3\xee\xbc\x15\x7c\x4f\xa2\xd1\x53\xfe\x7a\x8a\x6b"
	"\xad\x90\xd8\x35\x88\x3f\x6c\x23\x86\x6c\xac\x04\x7c\xc0\x46\x6f\x69\x20\xe8\xc2\x10\x2a\x0a\xbd\x52\x37\xe3\xe7\x32\x08\x09\x47"
	"\x5b\x99\xff\x50\x10\x4b\x30\x95\xb8\x55\xde\x46\x11\x75\x4c\xd5\xaf\x1d\xb9\x32\xbf\xd1\x96\x1b\x04\x9b\x0e\xa1\x0d\x7d\x78\x6f"
	"\xe9\xfd\x51\xc2\xca\x35\xee\x0a\x3d\x23\x32\xf1\x02\x54\x76\xfb\xb1\x28\x87\xda\xec\xf8\xd4\x37\x2a\x3e\xee\x76\x2e\x89\x23\xf7"
	"\x37\x95\xd1\x86\xda\xfb\x9f\xbd\x01\x27\xc6\x70\x55\xbd\xaa\xc5\xd3\xa0\xfd\xe5\x92\x6e\xdb\x7c\x6b\x24\xc8\x55\xb8\x47\xc7\x20"
	"\x27\x98\x19\x88\xba\x82\xad\xdf\x05\x50\x83\x70\xa9\x55\xd6\x72\x9d\xae\x72\x3d\x9d\xe7\x6b\xc9\x4d\x49\x3e\x4e\xc5\x9a\xc4\x9a"
	"\x5b\x91\xdf\xbe\xfd\x66\x20\xb2\x06\x39\x5f\xb7\xe5\xbb\xcc\xef\xff\xbb\xe4\x6e\x17\x6e\x05\x93\x9e\x9d\x76\x41\xfa\x8c\x16\x0b"
	"\x08\x63\x43\x14\x5f\x53\xd5\xa8\x22\x60\xbb\x07\xa1\x08\x0e\x9f\xf9\x38\x43\x02\xfe\x46\xe1\x76\xbd\x70\x04\xe3\x3e\xc1\xfc\x29"
	"\xcb\x6f\xf1\xae\x68\x38\x88\x45\x21\x3b\x47\x12\xa2\x3d\x22\x4c\x95\x31\x2c\x6a\x4a\x5d\x4e\xed\xc8\x4f\xff\x50\x80\x3e\x84\x46"
	"\x50\xc5\x18\x87\x8a\xde\xf6\xb0\xa9\x44\xe0\x5a\x24\x4f\xba\x1d\xbe\xcb\x52\x35\x46\x24\xb5\xfd\x08\x6e\xa2\xe2\xfc\x41\x13\x8a"
	"\xf2\x46\x35\xbd\xda\x10\x81\x10\x24\xb0\x0b\xae\xd3\xc2\xee\x9c\x62\x72\x4e\x9e\x25\x88\x33\x91\x28\x1e\x46\x7c\x78\x52\x66\x4e"
	"\x03\xdf\x51\x11\xee\xba\xf9\x4e\x1f\x63\x0b\x60\x7a\x10\x4c\x9b\x9b\x0d\x82\x64\x73\x1a\x48\x44\xd2\xa8\xd9\x6b\x05\xe3\xf3\x48"
	"\xbb\x9d\x8e\x49\x14\x6a\x04\x82\x4c\xb7\xd4\xe4\x05\x84\x80\x59\x0c\x40\xbb\x78\x9d\xd4\xbc\x72\xa5\x92\x4b\x4f\x3f\x13\x39\x8a"
	"\xf8\x07\xd0\xc1\x64\x4c\xe8\xf2\x3f\x0a\xb4\x87\x0d\x0e\x7b\xc1\x1e\x00\x67\x74\xaa\x3a\xe0\x55\xa0\xba\x5b\x5f\x9b\xf2\x87\x06"
	"\x57\x18\x21\xf0\x67\x61\xbe\xe0\x32\x57\xfe\xfc\x8e\x0c\xe5\x83\xcc\x7a\x28\xe5\x45\x75\x54\x8f\x9a\x70\xc1\xf2\x2e\x2b\x70\x33"
	"\x0c\x15\xd0\xd7\xcd\x8c\x39\xfa\x2d\x42\x9f\xac\xa7\x46\xd5\xc6\x6d\x55\xda\x41\xb3\x95\x02\x31\x76\xb3\xef\xb6\x8c\xf9\x06\xfa"
	"\x13\x95\x32\x10\x5f\x64\xc2\xa9\xb1\xbf\xe0\xda\x70\x97\x67\x86\xd7\x90\x15\x17\x67\x0d\x9d\x8f\xb4\xfc\x74\x07\x0e\xcd\x0d\x05"
	"\xbc\x9b\xbf\x2e\x3f\x58\x46\xfd\xe1\x8e\xf8\x43\x22\x2d\xaa\x54\x9d\x15\x47\x51\xe2\xba\x14\xd5\xf9\xef\xeb\x84\x8a\x10\x56\xc9"
	"\xd4\x83\xbf\xb2\xd8\xec\x40\x27\x61\xbc\x48\x67\xe8\xd2\x00\x0d\x55\x03\x85\x0c\xf0\x3c\xa7\x6a\x6e\xb5\x02\xad\xa9\x11\x39\x5d"
	"\x9d\xc3\x2d\x2c\xa2\xf3\xfe\x69\xa6\x48\x6e\x5e\x9b\x3a\x35\x3f\x75\x10\x22\x66\x66\x05\x8c\x6a\x7e\x8e\x9b\x2d\xca\x79\xd3\xda"
	"\x17\x61\x8a\x43\x24\xcd\x5c\xa4\x5e\x7d\x92\x85\xd4\xc0\x2d\x18\xf3\x1e\xf8\x69\x12\xbd\x50\x33\x05\xe5\xcf\x17\xf4\x71\x05\xeb"
	"\xcf\x75\x90\x2e\x6a\x00\x56\xb8\x9c\x51\x05\xf2\x6b\xfe\xd7\x79\xa8\x16\x61\xbb\x84\x3e\x71\xdf\xb1\xb1\xa8\xfd\x90\x64\x5b\xc0"
	"\xc0\x7a\x38\xa3\xfa\x4e\x32\xa7\x82\x93\x0b\x57\x62\x2c\x4d\x64\x59\x5e\x22\xd5\x2b\x90\x7d\x25\x50\xfe\x1e\xc1\x14\x70\xc0\x63"
	"\x80\xf2\xa4\x77\x59\xb0\xe8\x0e\x6e\x06\xdf\xa3\x73\x23\xcb\x68\x2d\x63\x1f\x8e\x01\xfa\x0a\x86\xc0\x9e\x45\xec\x67\x33\xf5\x21"
	"\xc0\x2f\xc2\x04\xd6\x19\x11\x16\x9b\x13\x29\xcf\xea\x18\x02\x22\x51\x20\xc2\xc6\x05\x93\x7a\x39\x57\x29\x55\x2d\xfb\x3c\xb0\x7c"
	"\xd7\x2d\xb3\x92\xc5\xc3\xe5\xa4\x72\x8c\x32\x5e\x59\xbe\x8c\x7d\x5d\x69\x26\x59\x02\x97\xe9\x1c\x83\x05\xc1\x99\xce\x2e\xc2\x24"
	"\xd3\x12\x44\x3c\xfe\xca\x92\x9b\x00\xa7\x6a\x40\x17\x41\xff\x9b\xe7\xf1\x5b\x8b\x1c\xfa\xf8\x80\x19\x22\x4b\x25\x27\xef\x62\x8e"
	"\x27\x85\xf0\xe7\xf4\x7b\x1d\x2e\x97\x00\xbb\x48\x4a\xf3\x8e\xb4\x4e\x5f\x22\x34\x83\xcd\x68\x4b\x8f\xe4\xb4\x45\x60\xa9\x72\xbe"
	"\xef\xdb\x9a\x3a\xc1\xf3\x67\x38\xbd\xf3\x17\x61\x04\x62\xed\x8c\x42\x49\xf1\x57\xb5\x83\x34\xa0\x19\x57\x07\x73\xe0\x12\xe2\x62"
	"\x24\x93\x39\xdc\x5c\x21\xbb\xd0\x49\x66\x53\x6c\xc4\x17\x96\xe1\xe6\x57\x4a\xb6\xe3\xc3\x00\x01\x5b\x71\x1e\xf9\x9c\xff\x05\x19"
	"\xe3\x00\xea\x83\x3f\x51\x42\x0d\xbe\x87\xcb\x3a\x5c\xbd\x4b\x4f\x5f\x32\x0d\x24\x30\x1d\xc9\x1e\xd6\x30\x05\x4e\x30\x7a\x16\x4b"
	"\x87\x21\x02\xab\x34\xaa\x5c\xc9\xb0\xdb\xa2\x42\x30\xdb\x0d\x6a\x66\x70\x77\xdd\x0a\x32\x68\xf9\xbd\xf8\x31\x01\x98\x8c\x21\x27"
	"\x87\x6c\xfc\x2d\xa2\x13\xec\x9c\xad\x0e\xbf\x63\x4d\xae\x0a\xd3\xd2\x1c\x81\xbf\x19\x29\x3c\x34\x57\xd3\xff\x59\x5f\x26\x98\xbc"
	"\x09\xf9\xf2\xe5\xcb\xd3\x87\xf7\x22\x3d\x89\x18\x42\x4f\x67\x50\x22\xd3\x38\x7f\x0e\xe5\xaf\x14\x8c\xcd\x03\x61\x9b\xc5\xc3\x5a"
	"\xd8\x80\xbf\x51\x46\x58\xb0\xdb\x1e\xc9\x32\xa6\x49\x37\xc6\x1b\xd3\xf5\x08\xb2\xef\xb8\x36\x8d\x21\x41\xa1\x0f\xc9\x9c\xd9\x82"
	"\x9a\xe3\x88\xb7\xd1\xa8\x2e\xc3\xc0\x10\x8e\x38\x88\x24\xa2\x01\x37\x0b\x16\x1c\x87\x57\x28\x05\x63\x87\x9c\x84\x61\x1d\x23\x51"
	"\x44\xb9\xa6\x80\xe7\x01\x22\xa6\xf7\x7d\x61\x5d\xbe\xf9\x91\x90\xc4\xba\xcd\x94\x58\x88\xe1\xe1\xe6\xd3\xcb\x80\x42\x95\x72\x5d"
	"\xe2\xc5\x76\x41\xc4\x74\xff\x64\xee\xe5\x79\x00\x87\x68\x6e\x15\x18\x74\x67\x4d\x3b\x67\x9c\xa0\xdb\x53\xb8\xe1\xfe\x68\x5e\xd7"
	"\x03"
;

void getLicenses(sf::Array<sf::License> &licenses)
{
	sf::SmallArray<sf::License, 32> localLicenses;

    sf::Array<char> decompressed;
    decompressed.resizeUninit(uncompressedSize);
	size_t realSize = sp_decompress_buffer(SP_COMPRESSION_ZSTD, decompressed.data, decompressed.size, compressedLicenses, sizeof(compressedLicenses) - 1);
    sf_assert(realSize == decompressed.size);

    jsi_value *val = jsi_parse_memory(decompressed.data, decompressed.size, nullptr);
    sf_assert(val);
    sp::readJson(val, localLicenses);
    jsi_free(val);

    for (sf::License &l : localLicenses) {
		licenses.push(std::move(l));
    }
}

}