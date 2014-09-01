#include "Text.h"
#include <vector>
#include <dav/gl.h>
#include <png/png.hpp>

namespace editor
{
	Text::Text(void)
	{
		png::image<png::rgba_pixel> img("charset.png");
		int w = img.get_width(), h = img.get_height();
		std::vector<BYTE> buf(w * h * 4);
		BYTE *p = &buf.front();

		for(int y = 0 ; y < h ; ++y)
		{
			for(int x = 0 ; x < w ; ++x)
			{
				auto c = img.get_pixel(x, y);
				*p++ = c.red;
				*p++ = c.green;
				*p++ = c.blue;
				*p++ = c.alpha;
			}
		}

		id_ = dav::gl::create_texture(&buf.front(), w, h, dav::gl::RGBA);
	}

	Text::~Text(void)
	{
		dav::gl::delete_texture(id_);
	}

	void Text::doDrawText(int x, int y, const std::string& s, DWORD c) const
	{
		if(c != 0xffffff) dav::gl::color(c);

		for(const char& ch : s)
		{
			doDrawChar(x, y, ch, 0xffffff);
			x += C_W;
		}

		if(c != 0xffffff) dav::gl::color(0xffffff);
	}

	void Text::doDrawChar(int x, int y, char ch_, DWORD c) const
	{
		if(c != 0xffffff) dav::gl::color(c);

		BYTE ch(ch_);

		float u = (ch % 16) / 16.0, v = (ch / 16) / 16.0;

		dav::gl::bind_texture(id_);

		dav::gl::draw_face2d(u, v, u + 0.0625, v + 0.0625, x, y, x + C_W, y + C_W);

		if(c != 0xffffff) dav::gl::color(0xffffff);
	}
}

