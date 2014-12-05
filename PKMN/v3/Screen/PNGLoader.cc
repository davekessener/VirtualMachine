#include <string>
#include <vector>
#include <cassert>
#include "PNGLoader.h"
#include <png/png.hpp>
#include "GLImageBuffer.h"

namespace dav { namespace pkmn { namespace screen {

DWORD PNGLoader::LoadPNG(const std::string& s)
{
	png::image<png::rgba_pixel> img(s);
	int w = img.get_width(), h = img.get_height();
	std::vector<BYTE> buf(w * h * 4);

	assert(w&&w==h&&!(w&(w-1)));

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

	return GLImageBuffer::getImage(&buf.front(), w);
}

}}}

