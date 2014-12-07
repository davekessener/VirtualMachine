#include <iostream>
#include <cassert>
#include "Surface.h"
#include "gl.h"

namespace dav { namespace pkmn { namespace screen {

void Surface::init(int x, int y, int w, int h)
{
	i_doInit();
	move(x, y);
	resize(w, h);

	dirty(true);
}

void Surface::move(int x, int y)
{
	x_ = x;
	y_ = y;

	updateAP();
}

void Surface::resize(int w, int h)
{
	w_ = w;
	h_ = h;

	i_doResize();

	dirty(true);
}

void Surface::update(uint d)
{
	i_doUpdate(d);

	for(auto& p : children_)
	{
		p->update(d);
	}
}

void Surface::prerender(void)
{
	for(auto& p : children_)
	{
		p->prerender();
	}

	if(dirty_)
	{
		i_doPrerender();
		dirty_ = false;
	}
}

void Surface::render(void)
{
	prerender();

	i_render();
}

void Surface::i_render(void)
{
	i_doRender();

	for(auto& p : children_)
	{
		p->i_render();
	}
}

void Surface::mouseClick(MouseButtons b, int x, int y, bool down)
{
	if(down) i_doMouseDown(b, x + dx_, y + dy_);
	else i_doMouseUp(b, x + dx_, y + dy_);
}

void Surface::mouseDrag(int x, int y)
{
	mousePos(x, y);
	i_doMouseDrag(x + dx_, y + dy_);
}

void Surface::mouseHover(int x, int y)
{
	Surface_ptr p = getControlAt(x, y);
	mousePos(x, y);
	p->i_doMouseHover(x + p->dx_, y + p->dy_);
}

void Surface::mouseScroll(int d)
{
	coords p(mousePos());
	getControlAt(p.x, p.y)->i_doMouseScroll(d);
}

void Surface::input(Controls k, bool down)
{
	if(down) i_doKeyDown(k);
	else i_doKeyUp(k);
}

Surface_ptr Surface::getControlAt(int x, int y)
{
	if(x < 0 || y < 0 || x >= w_ || y >= h_)
		assert(false);//throw std::string("Out of bound! getControlAt");
	
	return i_doGetControlAt(x, y);
}

Surface_ptr Surface::i_doGetControlAt(int x, int y)
{
	for(auto i(children_.rbegin()), e(children_.rend()) ; i != e ; ++i)
	{
		Surface_ptr p = static_cast<Surface_ptr>(*i);
		if(x >= p->x_ && y >= p->y_ && x < p->x_ + p->w_ && y < p->y_ + p->h_)
		{
			return p->getControlAt(x - p->x_, y - p->y_);
		}
	}

	return shared_from_this();
}

void Surface::addChild(Surface_ptr p, uint v)
{
//	child_t c(v, p);
//	children_.insert(std::upper_bound(children_.begin(), children_.end(), c), c);
	children_.push_back(p);
	p->setParent(shared_from_this());
}

void Surface::removeChild(DWORD id)
{
	for(auto i(children_.begin()), e(children_.end()) ; i != e ; ++i)
	{
		if(static_cast<Surface_ptr>(*i)->id_ == id)
		{
			static_cast<Surface_ptr>(*i)->setParent(Surface_ptr());
			children_.erase(i);
			break;
		}
	}
}

bool Surface::hasChild(DWORD id) const
{
	for(auto& p : children_)
	{
		if(p->id_ == id) return true;
	}

	return false;
}

void Surface::fillRect(int x1, int y1, int x2, int y2, color_t c)
{
	bbox bb(getAbsoluteAABB());

	x1 -= dx_; y1 -= dy_;
	x2 -= dx_; y2 -= dy_;

	if(x1 >= x2 || y1 >= y2) return;
	
	using std::min;
	using std::max;

	gl::fill_rect
	(
		max(x1, bb.pos.x),
		max(y1, bb.pos.y),
		min(x2, bb.pos.x + bb.vec.w),
		min(y2, bb.pos.y + bb.vec.h),
		c
	);
}

void Surface::draw(DWORD id, float u1, float v1, float u2, float v2, int x1, int y1, int x2, int y2)
{
	bbox bb(getAbsoluteAABB());

	x1 -= dx_; y1 -= dy_;
	x2 -= dx_; y2 -= dy_;

	if(x1 < bb.pos.x) { u1 += (bb.pos.x - x1) * (u2 - u1) / (x2 - x1); x1 = bb.pos.x; }
	if(y1 < bb.pos.y) { v1 += (bb.pos.y - y1) * (v2 - v1) / (y2 - y1); y1 = bb.pos.y; }
	if(x2 > bb.pos.x + bb.vec.w)
		{ u2 -= (x2 - (bb.pos.x + bb.vec.w)) * (u2 - u1) / (x2 - x1); x2 = bb.pos.x + bb.vec.w; }
	if(y2 > bb.pos.y + bb.vec.h)
		{ v2 -= (y2 - (bb.pos.y + bb.vec.h)) * (v2 - v1) / (y2 - y1); y2 = bb.pos.y + bb.vec.h; }
	
	if(x1 >= x2 || y1 >= y2) return;
	
	if(id) gl::bind_texture(id);
	gl::draw_face2d(u1, v1, u2, v2, x1, y1, x2, y2);
}

void Surface::drawString(const std::string& s, int x, int y, color_t c, uint cw)
{
	gl::color(c.rgb());
	gl::bind_texture(charsetID());

	for(const char& ch : s)
	{
		if(ch != ' ')
		{
			float u((ch % 16) / 16.0), v((ch / 16) / 16.0);
			draw(0, u, v, u + 0.0625, v + 0.0625, x, y, x + cw, y + cw);
		}
		x += cw;
	}

	gl::color(0xffffff);
}

bool Surface::dirty(bool force)
{
	if(force)
	{
		return dirty_ = true;
	}
	else
	{
		if(dirty_) return true;

		for(auto& p : children_)
		{
			if(p->dirty()) return true;
		}

		return false;
	}
}

void Surface::invalidate(void)
{
	dirty_ = true;

	for(auto& p : children_)
	{
		p->invalidate();
	}
}

void Surface::active(bool a)
{
	active_ = a;

	for(auto& p : children_)
	{
		p->active(active_);
	}
}

bbox Surface::getAbsoluteAABB(void)
{
	bbox bb(coords(x_, y_), size(w_, h_));
	Surface_ptr pp = shared_from_this();

	while(pp->hasParent())
	{
		pp = pp->parent();
		bbox p(pp->getAbsoluteAABB());
		bb.pos.x += p.pos.x; bb.pos.y += p.pos.y;
		if(bb.pos.x < p.pos.x) { bb.vec.w -= p.pos.x - bb.pos.x; bb.pos.x = p.pos.x; }
		if(bb.pos.y < p.pos.y) { bb.vec.h -= p.pos.y - bb.pos.y; bb.pos.y = p.pos.y; }
		if(bb.vec.w > p.vec.w) bb.vec.w = p.vec.w;
		if(bb.vec.h > p.vec.h) bb.vec.h = p.vec.h;
	}

	return bb;
}

void Surface::updateAP( )
{
	dx_ = -x_; dy_ = -y_;

	Surface_ptr p = shared_from_this();

	while(p->hasParent())
	{
		p = p->parent();
		dx_ -= p->x_;
		dy_ -= p->y_;
	}
}

}}}

