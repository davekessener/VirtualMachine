#include "Text.h"
#include <vector>
#include <cassert>
#include <dav/gl.h>
#include "surface/PNGLoader.h"

#define MXT_CHARSETPATH "charset.png"

namespace editor
{
	Text::Text(void) : id_(surface::PNGLoader::loadPNG(MXT_CHARSETPATH))
	{
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

