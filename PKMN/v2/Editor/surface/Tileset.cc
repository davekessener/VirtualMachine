#include "Tileset.h"
#include "../Tileset.h"
#include "Button.h"
#include "IconBuffer.h"
#include <dav/gl.h>
#include <dav/Logger.h>

#define MXT_MIN(a,b) (((long)(a)<(long)(b))?(a):(b))

#define MXT_GRIDPATH "icons/grid.png"

namespace editor { namespace surface {

typedef ::editor::Tileset TS;
using dav::gl::fill_rect;
using dav::gl::draw_rect;
using dav::gl::draw_line;
using dav::gl::draw_face2d;

void Tileset::i_doInit(void)
{
	int dx = 0;

	Button *b = new Button("", []{});
	b->~Button();
	Surface_ptr p(new(b) Button(
		IconBuffer::registerIcon(MXT_GRIDPATH), [this, b]{ b->force(TS::GRID = !TS::GRID); dirty(); }));
	p->init(dx, 0, 16, 16);
	addChild(p);

	s_.x = 64 - (int)width() / 16;
	s_.y = 64 - (int)height() / 16 - 1;

	if(s_.x < 0) s_.x = 0;
	if(s_.y < 0) s_.y = 0;

	id_ = TS::get();
	down_ = false;
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

	fill_rect(p.x, p.y, q.x, p.x + 16, 0xf0f0f0);

	p.y += 16;

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

	{
		int dx = o_.x * 16, dy = o_.y * 16;
		int w = MXT_MIN(width(), 1024 - dx), h = MXT_MIN(height(), 1024 - dy);

		draw_face2d(dx / 1024.0, dy / 1024.0, (dx + w) / 1024.0, (dy + h) / 1024.0, p.x, p.y, p.x + w, p.y + h);
	}

	{
		int x1 = id_.left() % 64, y1 = id_.left() / 64;
		int w = id_.width() * 16, h = id_.height() * 16;

		if(    x1 >= o_.x && (x1 - o_.x) * 16 < (int)width()
			&& y1 >= o_.y && (y1 - o_.y) * 16 < (int)height())
		{
			int x = (x1 - o_.x) * 16, y = (y1 - o_.y) * 16;
			draw_rect(p.x + x + TS::GRID, p.y + y + TS::GRID,
				p.x + MXT_MIN(width(), x + w), p.y + MXT_MIN(height(), y + h), 0xff0000);
		}
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

void Tileset::i_doMouseDown(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && isOver(x, y))
	{
		x = o_.x + x / 16;
		y = o_.y + (y - 16) / 16;

		if(x < 0 || x >= 64 || y < 0 || y >= 64) return;

		down_ = true;
		did_ = x + y * 64;
		id_ = TileBlock(did_, did_);

		TS::set(id_);

		dirty();
	}
}

void Tileset::i_doMouseMove(int x, int y)
{
	if(down_ && isOver(x, y))
	{
		x = o_.x + x / 16;
		y = o_.y + (y - 16) / 16;

		if(x < 0 || x >= 64 || y < 0 || y >= 64) return;
		
		id_ = TileBlock(did_, x + y * 64);

		TS::set(id_);

		dirty();
	}
}

void Tileset::i_doMouseUp(MouseButtons b, int x, int y)
{
	down_ = false;
	dirty();
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

