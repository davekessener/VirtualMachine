#include "Tileset.h"
#include "../Tileset.h"
#include <dav/gl.h>
#include <dav/Logger.h>

namespace editor { namespace surface {

void Tileset::i_doInit(void)
{
	s_.x = 64 - (int)width() / 16;
	s_.y = 64 - (int)height() / 16;

	if(s_.x < 0) s_.x = 0;
	if(s_.y < 0) s_.y = 0;
}

void Tileset::i_doUpdate(int d)
{
}

void Tileset::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	for(int y = 0, Y = (height() + 15) / 16 ; y < Y ; ++y)
	{
		for(int x = 0, X = (width() + 15) / 15 ; x < X ; ++x)
		{
#define min(a,b) (((long)(a)<(long)(b))?(a):(b))
			dav::gl::draw_rect(p.x + x * 16, p.y + y * 16,
				p.x + min(width(), (x + 1) * 16), p.y + min(height(), (y + 1) * 16),
				(x + (y & 1)) & 1 ? 0xff00ff : 0x00ff00);
#undef min
		}
	}

	::editor::Tileset::bind();

#define min(a,b) (((long)(a)<(long)(b))?(a):(b))
	int dx = o_.x * 16, dy = o_.y * 16;
	int w = min(width(), 1024 - dx), h = min(height(), 1024 - dy);
#undef min

	dav::gl::draw_face2d(dx / 1024.0, dy / 1024.0, (dx + w) / 1024.0, (dy + h) / 1024.0, p.x, p.y, p.x + w, p.y + h);
}

point Tileset::getScrollSizes(void) const
{
	return s_;
}

void Tileset::setScrollPos(int x, int y)
{
	o_.x = x;
	o_.y = y;
	dirty();
}

}}

