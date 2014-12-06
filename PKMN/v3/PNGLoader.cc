#include <cassert>
#include "PNGLoader.h"
#include <png/png.hpp>
#include "GLImageBuffer.h"

namespace dav { namespace pkmn { namespace screen {

DWORD PNGLoader::LoadPNG(const std::string& s)
{
	std::vector<BYTE> buf;

	int w = LoadRawPNG(s, buf);

	return GLImageBuffer::getImage(&buf.front(), w);
}

int PNGLoader::LoadRawPNG(const std::string& s, std::vector<BYTE>& buf)
{
	png::image<png::rgba_pixel> img(s);
	int w = img.get_width(), h = img.get_height();
	buf.resize(w * h * 4);

	assert(w>0&&w==h&&!(w&(w-1)));

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
	
	return w;
}

}}}

