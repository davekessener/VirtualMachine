#include "Scrollable.h"
#include <cassert>
#include <dav/Logger.h>
#include <dav/gl.h>
#include "IconBuffer.h"
#include "ButtonHelper.h"
#include "Button.h"

#define MXT_SW 16

#define MXT_MESHPATH "icons/scroll_fill.png"

namespace editor { namespace surface {

void ScrollBar::i_doInit(void)
{
	f_ = width() < height();
	pos_ = 0;
	mesh_ = IconBuffer::registerIcon(MXT_MESHPATH);
	down_ = false;
	s_ = f_ ? width() : height();

	Surface_ptr p(new Button("", [this]{ setPos(pos_ - 1); }));
	p->init(0, 0, s_, s_);
	addChild(p);

	p.reset(new Button("", [this] { setPos(pos_ + 1); }));
	p->init(width() - s_, height() - s_, s_, s_);
	addChild(p);
}

void ScrollBar::i_doUpdate(int d)
{
}

void ScrollBar::i_doPrerender(void)
{
	int n = f_ ? height() : width();
	w_ = n / (gran_ + 1);
	int o = w_ * pos_;

	point x1(s_ + o,      0), y1(0, s_ + o);
	point x2(s_ + o + w_, width()), y2(height(), s_ + o + w_);

	p1_.x = f_ ? x1.y : x1.x; p1_.y = f_ ? y1.y : y1.x;
	p2_.x = f_ ? x2.y : x2.x; p2_.y = f_ ? y2.y : y2.x;
}

void ScrollBar::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	for(int y = 0, Y = (height() + 15) / 16 ; y < Y ; ++y)
	{
		for(int x = 0, X = (width() + 15) / 16 ; x < X ; ++x)
		{
#define min(a,b) (((long)(a)<(long)(b))?(a):(b))
			IconBuffer::renderIcon(mesh_, 
				p.x + x * 16,                     p.y + y * 16, 
				p.x + min(width(), (x + 1) * 16), p.y + min(height(), (y + 1) * 16));
#undef min
		}
	}

	if(gran_ > 0)
	{
		ButtonHelper::renderButton(p.x + p1_.x, p.y + p1_.y, p.x + p2_.x, p.y + p2_.y, down_);
	}
}

void ScrollBar::i_doMouseDown(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && isOver(x, y))
	{
		down_ = true;
		i_doMouseMove(x, y);
	}
}

void ScrollBar::i_doMouseMove(int x, int y)
{
	if(down_)
	{
		setPos(getPosFrom(x, y));
	}
}

void ScrollBar::i_doMouseUp(MouseButtons b, int x, int y)
{
	down_ = false;
}

// # ===========================================================================

void Scrolling::i_doInit(void)
{
	if(!content_)
	{
		LOG("ERR: Scrolling has no content.");
		throw std::string("scrolling has no content!");
	}

	content_->init(0, 0, width() - MXT_SW, height() - MXT_SW);
	addChild(content_);

	smax_ = content_->getScrollSizes();

	hor_.reset(new ScrollBar(
		[this](int p) { if(p != scur_.x) content_->setScrollPos(scur_.x = p, scur_.y); }, smax_.x));
	hor_->init(0, height() - MXT_SW, width() - MXT_SW, MXT_SW);
	addChild(hor_);

	ver_.reset(new ScrollBar(
		[this](int p) { if(p != scur_.y) content_->setScrollPos(scur_.x, scur_.y = p); }, smax_.y));
	ver_->init(width() - MXT_SW, 0, MXT_SW, height() - MXT_SW);
	addChild(ver_);

	scur_ = point(0, 0);

	content_->setScrollPos(0, 0);
//
//	mesh_ = IconBuffer::registerIcon(MXT_MESHPATH);
}

void Scrolling::i_doUpdate(int d)
{
	point p = content_->getScrollSizes();
	
	if(p != smax_)
	{
		point t = scur_;
		smax_ = p;

		hor_->setScale(smax_.x);
		ver_->setScale(smax_.y);

		scur_.x = hor_->getPos();
		scur_.y = ver_->getPos();
		
		if(t != scur_) content_->setScrollPos(scur_.x, scur_.y);
		
		dirty();
	}
}

void Scrolling::i_doPrerender(void)
{
//	vs_ = hs_ = 0;
//
//	int sx = width() - 2 * MXT_SW, sy = height() - 2 * MXT_SW;
//
//	if(smax_.x > 0)
//	{
//		hs_ = sx / (smax_.x + 1);
//	}
//
//	if(smax_.y > 0)
//	{
//		vs_ = sy / (smax_.y + 1);
//	}
}

void Scrolling::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

//	ButtonHelper::renderButton(q.x - MXT_SW, q.y - MXT_SW, q.x, q.y);
	dav::gl::draw_rect(q.x - MXT_SW, q.y - MXT_SW, q.x, q.y, 0x404040);

//	for(int i = 1, e = width() / MXT_SW ; i < e ; ++i)
//	{
//		IconBuffer::renderIcon(mesh_, p.x + i * MXT_SW, q.y - MXT_SW, p.x + (i + 1) * MXT_SW, q.y);
//	}
//
//	for(int i = 1, e = height() / MXT_SW ; i < e ; ++i)
//	{
//		IconBuffer::renderIcon(mesh_, q.x - MXT_SW, p.y + i * MXT_SW, q.x, p.y + (i + 1) * MXT_SW);
//	}
//
//	ButtonHelper::renderButton(p.x, q.y - MXT_SW, p.x + MXT_SW, q.y);
//	ButtonHelper::renderButton(q.x - MXT_SW, p.y, q.x, p.y + MXT_SW);
//	ButtonHelper::renderButton(q.x - MXT_SW, q.y - MXT_SW, q.x, q.y);
//
//	if(hs_ > 0)
//	{
//		int dx = MXT_SW + scur_.x * hs_;
//		ButtonHelper::renderButton(p.x + dx, q.y - MXT_SW, p.x + dx + hs_, q.y);
//	}
//
//	if(vs_ > 0)
//	{
//		int dy = MXT_SW + scur_.y * vs_;
//		ButtonHelper::renderButton(q.x - MXT_SW, p.y + dy, q.x, p.y + dy + vs_);
//	}
}

}}

