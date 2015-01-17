#include <cassert>
#include "PNGLoader.h"
#include <png/png.hpp>
#include "GLImageBuffer.h"

namespace dav { namespace pkmn { namespace screen {

DWORD PNGLoader::LoadSquarePNG(const std::string& s)
{
	std::vector<BYTE> buf;

	raw_info i = LoadRawPNG(s, buf);

	assert(i.w>0&&i.w==i.h&&!(i.w&(i.w-1)));

	return GLImageBuffer::GetImage(&buf.front(), i.w);
}

text_info PNGLoader::LoadPNG(const std::string& s)
{
	std::vector<BYTE> buf;

	raw_info i = LoadRawPNG(s, buf);

	text_info t;
	t.id = GLImageBuffer::GetImage(&buf.front(), i.s);
	t.u1 = t.v1 = 0;
	t.u2 = i.w / (float)i.s;
	t.v2 = i.h / (float)i.s;
	t.size = i.s;

	return t;
}

PNGLoader::raw_info PNGLoader::LoadRawPNG(const std::string& fn, std::vector<BYTE>& buf)
{
	png::image<png::rgba_pixel> img(fn);
	int w = img.get_width(), h = img.get_height();
	int s = 1;

	assert(w>0&&h>0);

	while(s < w && s < h) s <<= 1;

	buf.resize(s * s * 4);

	if(s == w && s == h)
	{
		BYTE *p = &buf.front();
		for(int y = 0 ; y < h ; ++y)
		{
			for(int x = 0; x < w ; ++x)
			{
				auto c(img.get_pixel(x, y));
				*p++ = c.red;
				*p++ = c.green;
				*p++ = c.blue;
				*p++ = c.alpha;
			}
		}
	}
	else
	{
		for(int y = 0 ; y < h ; ++y)
		{
			for(int x = 0 ; x < w ; ++x)
			{
				auto c(img.get_pixel(x, y));
				buf[(x + y * s) * 4 + 0] = c.red;
				buf[(x + y * s) * 4 + 1] = c.green;
				buf[(x + y * s) * 4 + 2] = c.blue;
				buf[(x + y * s) * 4 + 3] = c.alpha;
			}
		}
	}
	
	raw_info ri;
	ri.w = w;
	ri.h = h;
	ri.s = s;

	return ri;
}

}}}

