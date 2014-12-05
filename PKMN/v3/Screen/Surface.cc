#include <iostream>
#include "Surface.h"
#include "gl.h"

namespace dav { namespace pkmn { namespace screen {

void Surface::init(int x, int y, uint w, uint h)
{
	i_doInit();
	move(x, y);
	resize(w, h);
}

void Surface::move(int x, int y)
{
	x_ = x;
	y_ = y;
}

void Surface::resize(uint w, uint h)
{
	w_ = w;
	h_ = h;

	i_doResize();
}

void Surface::update(uint d)
{
	i_doUpdate(d);

	for(auto& p : children_)
	{
		static_cast<Surface_ptr>(p)->update(d);
	}
}

void Surface::render(void)
{
	if(dirty_)
	{
		i_doRender();
		dirty_ = false;
	}

	for(auto& p : children_)
	{
		static_cast<Surface_ptr>(p)->render();
	}
}

void Surface::addChild(Surface_ptr p, uint v)
{
	child_t c(v, p);
	children_.insert(std::upper_bound(children_.begin(), children_.end(), c), c);
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
		if(static_cast<Surface_ptr>(p)->id_ == id) return true;
	}

	return false;
}

void Surface::fillRect(int x1, int y1, int x2, int y2, color_t c)
{
	x1 += x_; y1 += y_;
	x2 += x_; y2 += y_;

	if(hasParent())
	{
		parent()->fillRect(x1, y1, x2, y2, c);
	}
	else
	{
		if(x2 <= x_ || y2 <= y_ || x1 >= x_ + w_ || y1 >= y_ + h_) return;
		
		using std::min;
		using std::max;
		
		gl::fill_rect(max(x1, x_), max(y1, y_), min(x2, x_ + w_ - 1), min(y2, y_ + h_ - 1), c.rgb());
	}
}

void Surface::draw(DWORD id, float u1, float v1, float u2, float v2, int x1, int y1, int x2, int y2)
{
	bbox bb(getAbsoluteAABB());
	coords o(getAbsolutePosition());

	x1 += o.x; y1 += o.y;
	x2 += o.x; y2 += o.y;

	if(x1 < bb.pos.x) { u1 += (bb.pos.x - x1) * (u2 - u1) / (x2 - x1); x1 = bb.pos.x; }
	if(y1 < bb.pos.y) { v1 += (bb.pos.y - y1) * (v2 - v1) / (y2 - y1); y1 = bb.pos.y; }
	if(x2 > bb.pos.x + bb.vec.w)
		{ u2 -= (x2 - (bb.pos.x + bb.vec.w)) * (u2 - u1) / (x2 - x1); x2 = bb.pos.x + bb.vec.w; }
	if(y2 > bb.pos.y + bb.vec.h)
		{ v2 -= (y2 - (bb.pos.y + bb.vec.h)) * (v2 - v1) / (y2 - y1); y2 = bb.pos.y + bb.vec.h; }
	
	if(x1 >= x2 || y1 >= y2) return;
	
	gl::bind_texture(id);
	gl::draw_face2d(u1, v1, u2, v2, x1, y1, x2, y2);
}

void Surface::drawString(const std::string& s, int x, int y, color_t c, uint cw)
{
	DWORD tid(charsetID());

	gl::color(c.rgb());

	for(const char& ch : s)
	{
		if(ch != ' ')
		{
			float u((ch % 16) / 16.0), v((ch / 16) / 16.0);
			draw(tid, u, v, u + 0.0625, v + 0.0625, x, y, x + cw, y + cw);
		}
		x += cw;
	}
}

bool Surface::dirty(bool force)
{
	if(force == true)
	{
		if(!dirty_ && hasParent()) parent()->dirty(true);
		dirty_ = true;
	}

	return dirty_;
}

void Surface::invalidate(void)
{
	dirty_ = true;

	for(auto& p : children_)
	{
		static_cast<Surface_ptr>(p)->invalidate();
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

coords Surface::getAbsolutePosition( )
{
	coords p(x_, y_);
	Surface_ptr pp = shared_from_this();

	while(pp->hasParent())
	{
		pp = pp->parent();
		p.x += pp->x_; p.y += pp->y_;
	}

	return p;
}

}}}

