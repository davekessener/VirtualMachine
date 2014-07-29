#include <fstream>
#include <algorithm>
#include <cassert>
#include "simple_bmp.h"

namespace lib { namespace img { namespace simple {

struct dib_BITMAPINFOHEADER_t
{
	std::uint32_t headersize;
	std::int32_t width;
	std::int32_t height;
	std::uint16_t planes;
	std::uint16_t bpp;
	std::uint32_t compression;
	std::uint32_t size;
	struct
	{
		std::int32_t dx;
		std::int32_t dy;
	} __attribute__((packed)) resolution;
	std::uint32_t palettecount;
	std::uint32_t importantcolors;
} __attribute__((packed));

struct bmp_header_t
{
	std::uint16_t id;
	std::uint32_t filesize;
	std::uint32_t reserved;
	std::uint32_t offset;
} __attribute__((packed));

image read_simple_bmp(const std::string& fn, bool flipY)
{
	std::ifstream in(fn.c_str());

	assert(in.is_open());

	bmp_header_t bmp;
	dib_BITMAPINFOHEADER_t dib;

	in.read(reinterpret_cast<char *>(&bmp), sizeof(bmp));
	in.read(reinterpret_cast<char *>(&dib), sizeof(dib));

	image img;
	img.width = dib.width;
	img.height = dib.height;
	img.bpp = dib.bpp;
	img.data.resize(dib.width * dib.height);
	in.read(reinterpret_cast<char *>(&*img.data.begin()), img.data.size() * dib.bpp / 8);

	in.close();

	std::transform(img.data.cbegin(), img.data.cend(), img.data.begin(),
#define O(d) ((c>>(8*(d)))&0xff)
#define A(u,v,w,x) (((u)<<24)|((v)<<16)|((w)<<8)|(x))
		[](const DWORD& c) -> DWORD { return DWORD(A(O(3), O(0), O(1), O(2))); });
#undef A
#undef O
	
	if(flipY);
	{
		std::vector<DWORD> v(img.width);
		for(int i = 0, w = dib.width, h = dib.height ; i < h / 2 ; ++i)
		{
			std::copy_n(img.data.cbegin() + i * w, w, v.begin());
			std::copy_n(img.data.cbegin() + (h - i - 1) * w, w, img.data.begin() + i * w);
			std::copy_n(v.cbegin(), w, img.data.begin() + (h - i - 1) * w);
		}
	}

	return img;
}

}}}

