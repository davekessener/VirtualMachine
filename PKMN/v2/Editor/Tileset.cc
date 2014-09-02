#include "Tileset.h"
#include <dav/gl.h>
#include <png/png.hpp>

#define MXT_TILESETPATH "tileset.png"

namespace editor
{
	Tileset::Tileset(void)
	{
		png::image<png::rgba_pixel> img(MXT_TILESETPATH);
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

		id_ = dav::gl::create_texture(&buf.front(), w, h, dav::gl::RGBA);
	}

	Tileset::~Tileset(void)
	{
		dav::gl::delete_texture(id_);
	}

	void Tileset::doBind(void) const
	{
		dav::gl::bind_texture(id_);
	}
}

