#include "Tileset.h"
#include "../Tileset.h"
#include <dav/gl.h>
#include <dav/Logger.h>

#define MXT_MIN(a,b) (((long)(a)<(long)(b))?(a):(b))

namespace editor { namespace surface {

typedef ::editor::Tileset TS;
using dav::gl::fill_rect;
using dav::gl::draw_rect;
using dav::gl::draw_line;
using dav::gl::draw_face2d;

void Tileset::i_doInit(void)
{
	s_.x = 64 - (int)width() / 16;
	s_.y = 64 - (int)height() / 16;

	if(s_.x < 0) s_.x = 0;
	if(s_.y < 0) s_.y = 0;

	id_ = TS::get();
}

void Tileset::i_doUpdate(int d)
{
	if(id_ != TS::get())
	{
		id_ = TS::get();
		dirty();
	}
}

void Tileset::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	for(int y = 0, Y = (height() + 15) / 16 ; y < Y ; ++y)
	{
		for(int x = 0, X = (width() + 15) / 16 ; x < X ; ++x)
		{
			fill_rect(p.x + x * 16, p.y + y * 16,
				p.x + MXT_MIN(width(), (x + 1) * 16), p.y + MXT_MIN(height(), (y + 1) * 16),
				(x + (y & 1)) & 1 ? 0xff00ff : 0x00ff00);
		}
	}

	TS::bind();

	int dx = o_.x * 16, dy = o_.y * 16;
	int w = MXT_MIN(width(), 1024 - dx), h = MXT_MIN(height(), 1024 - dy);

	draw_face2d(dx / 1024.0, dy / 1024.0, (dx + w) / 1024.0, (dy + h) / 1024.0, p.x, p.y, p.x + w, p.y + h);

	int x1 = id_ % 64, y1 = id_ / 64;

	if(    x1 >= o_.x && (x1 - o_.x) * 16 < (int)width()
		&& y1 >= o_.y && (y1 - o_.y) * 16 < (int)height())
	{
		int x = (x1 - o_.x) * 16, y = (y1 - o_.y) * 16;
		draw_rect(p.x + x + TS::GRID, p.y + y + TS::GRID,
			p.x + MXT_MIN(width(), x + 16), p.y + MXT_MIN(height(), y + 16), 0xff0000);
	}

	if(TS::GRID)
	{
		for(int y = 0, Y = (height() + 15) / 16 ; y < Y ; ++y)
		{
			draw_line(p.x, p.y + y * 16, q.x, p.y + y * 16, 0);
		}
			
		for(int x = 0, X = (width() + 15) / 16 ; x < X ; ++x)
		{
			draw_line(p.x + x * 16, p.y, p.x + x * 16, q.y, 0);
		}
	}
}

void Tileset::i_doMouseUp(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && isOver(x, y))
	{
		int id = (x / 16 + o_.x) + (y / 16 + o_.y) * 64;
		TS::set(id);
		dirty();
	}
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

