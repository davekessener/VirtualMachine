#include "Viewer.h"
#include "../Controller.h"
#include "../Tileset.h"
#include "Button.h"
#include "IconBuffer.h"
#include <dav/gl.h>

#define MXT_SPLITPATH "icons/split.png"
#define MXT_GRIDPATH "icons/grid.png"
#define MXT_UNDOPATH "icons/undo.png"
#define MXT_REDOPATH "icons/redo.png"
#define MXT_SAVEPATH "icons/save.png"

#define MXT_MIN(a,b) (((long)(a)<(long)(b))?(a):(b))

namespace editor { namespace surface {

typedef ::editor::Tileset TS;

void Viewer::i_doInit(void)
{
	layer_ = 0;
	down_ = false;
	o_ = point(0, 0);

	int dx = 0;

	Surface_ptr p(new Button(IconBuffer::registerIcon(MXT_SAVEPATH), []{ Controller::save(); }));
	p->init(dx, 0, 16, 16);
	addChild(p);

	dx += 20;

	btns_[0].reset(new Button("1", [this]{ setLayer(0); }));
	btns_[0]->init(dx, 0, 16, 16);
	addChild(btns_[0]);

	dx += 16;

	btns_[1].reset(new Button("2", [this]{ setLayer(1); }));
	btns_[1]->init(dx, 0, 16, 16);
	addChild(btns_[1]);

	dx += 16;

	btns_[2].reset(new Button("3", [this]{ setLayer(2); }));
	btns_[2]->init(dx, 0, 16, 16);
	addChild(btns_[2]);

	dx += 16;

	btns_[3].reset(new Button("4", [this]{ setLayer(3); }));
	btns_[3]->init(dx, 0, 16, 16);
	addChild(btns_[3]);

	dx += 20;

	Button *b = new Button("", []{});
	b->~Button();
	new(b) Button(IconBuffer::registerIcon(MXT_SPLITPATH), [b]{ b->force(Controller::SPLIT = !Controller::SPLIT); });
	p.reset(b);
	p->init(dx, 0, 16, 16);
	addChild(p);

	dx += 16;
	
	b = new Button("", []{});
	b->~Button();
	new(b) Button(IconBuffer::registerIcon(MXT_GRIDPATH), [b]{ b->force(Controller::GRID = !Controller::GRID); });
	p.reset(b);
	p->init(dx, 0, 16, 16);
	addChild(p);

	dx += 20;

	p.reset(new Button(IconBuffer::registerIcon(MXT_UNDOPATH), [this]{ Controller::undo(); dirty(); }));
	p->init(dx, 0, 16, 16);
	addChild(p);

	dx += 16;

	p.reset(new Button(IconBuffer::registerIcon(MXT_REDOPATH), [this]{ Controller::redo(); dirty(); }));
	p->init(dx, 0, 16, 16);
	addChild(p);

	getButton<Button>(0)->force(true);
}

void Viewer::i_doUpdate(int d)
{
}

void Viewer::i_doPrerender(void)
{
}

void Viewer::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	dav::gl::fill_rect(p.x, p.y, q.x, p.y + 16, 0xf0f0f0);

	p.y += 16;

	TS::bind();

	const auto &m(Controller::getMap());

	dav::gl::fill_rect(p.x, p.y, p.x + MXT_MIN(width(), m.width() * 16), p.y + MXT_MIN(height(), m.height() * 16), 0);

	for(int i = 0 ; i < (Controller::SPLIT ? layer_ + 1 : 4) ; ++i)
	{
		const auto &v(m[i]);
	
		if(Controller::SPLIT && i == layer_)
		{
			dav::gl::color(1.0, 1.0, 1.0, 0.5);
			dav::gl::fill_rect(p.x, p.y, q.x, q.y);
			dav::gl::color(1, 1, 1, 1);
		}

		for(int y = 0, Y = (height() + 15) / 16 ; y < Y ; ++y)
		{
			for(int x = 0, X = (width() + 15) / 16 ; x < X ; ++x)
			{
				if(x + o_.x >= (int)m.width() || y + o_.y >= (int)m.height()) continue;

				DWORD id = v.at(x + o_.x + (y + o_.y) * m.width());

				if(!id) continue;

				int dx = (id % 64) * 16, dy = (id / 64) * 16;
				int w = MXT_MIN(width(), (x + 1) * 16) - x * 16,
					h = MXT_MIN(height(), (y + 1) * 16) - y * 16;
				dav::gl::draw_face2d(dx / 1024.0, dy / 1024.0, (dx + w) / 1024.0, (dy + h) / 1024.0,
									 p.x + x * 16, p.y + y * 16, p.x + x * 16 + w, p.y + y * 16 + h);
			}
		}
	}

	if(Controller::GRID)
	{
		for(int y = 0, Y = MXT_MIN((height() + 15) / 16, m.height()), w = m.width() * 16 ; y < Y ; ++y)
		{
			dav::gl::draw_line(p.x, p.y + y * 16, MXT_MIN(q.x, p.x + w), p.y + y * 16, 0);
		}

		for(int x = 0, X = MXT_MIN((width() + 15) / 16, m.width()), h = m.height() * 16 ; x < X ; ++x)
		{
			dav::gl::draw_line(p.x + x * 16, p.y, p.x + x * 16, MXT_MIN(q.y, p.y + h), 0);
		}
	}
}

void Viewer::i_doMouseDown(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && isOver(x, y))
	{
		down_ = true;
		i_doMouseMove(x, y);
	}
}

void Viewer::i_doMouseMove(int x, int y)
{
	if(down_)
	{
		const auto &m(Controller::getMap());
		x /= 16;
		y /= 16;

		--y;

		if(x >= 0 && x < (int)m.width() && y >= 0 && y < (int)m.height())
		{
			Controller::set(layer_, x, y, TS::get());
			dirty();
		}
	}
}

void Viewer::i_doMouseUp(MouseButtons b, int x, int y)
{
	const auto &m(Controller::getMap());
	DWORD id = TS::get().left();
	std::vector<point> rvec;

	auto isIn = [&rvec](int x, int y) { return std::find(rvec.begin(), rvec.end(), point(x, y)) != rvec.end(); };

	std::function<void(int, int)> fill = [this, &m, &rvec, &isIn, &fill](int x, int y)
	{
		DWORD r = Controller::get(layer_, x, y);

		rvec.push_back(point(x, y));

		if(x > 0 && Controller::get(layer_, x - 1, y) == r && !isIn(x - 1, y)) fill(x - 1, y);
		if(x < (int)m.width() - 1 && Controller::get(layer_, x + 1, y) == r && !isIn(x + 1, y)) fill(x + 1, y);
		if(y > 0 && Controller::get(layer_, x, y - 1) == r && !isIn(x, y - 1)) fill(x, y - 1);
		if(y < (int)m.height() - 1 && Controller::get(layer_, x, y + 1) == r && !isIn(x, y + 1)) fill(x, y + 1);
	};
	
	x /= 16;
	y /= 16;

	--y;
	
	if(x >= 0 && x < (int)m.width() && y >= 0 && y < (int)m.height())
	{
		switch(b)
		{
			case MouseButtons::LEFT:
				break;
			case MouseButtons::RIGHT:
				TS::set(m[layer_].at(x + y * m.width()));
				break;
			case MouseButtons::MIDDLE:
				Controller::commitBuffer();
				fill(x, y);
				for(const point& p : rvec) Controller::setBuffer(layer_, p.x, p.y, id);
				break;
			default:
				break;
		}
	}

	Controller::commitBuffer();
	down_ = false;
	dirty();
}

point Viewer::getScrollSizes(void) const
{
	return point(0, 0);
}

void Viewer::setScrollPos(int dx, int dy)
{
	o_ = point(dx, dy);
	dirty();
}

void Viewer::setLayer(int i)
{
	assert(i>=0&&i<4);

	if(i != layer_)
	{
		getButton<Button>(layer_)->force(false);
		getButton<Button>(layer_ = i)->force(true);
		dirty();
	}
}

}}

