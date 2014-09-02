#include "StringList.h"
#include "../Text.h"
#include <dav/gl.h>
#include <dav/Logger.h>

#define MXT_H (Text::C_W * 3 / 2)

namespace editor { namespace surface {

void StringList::i_doInit(void)
{
	WFocus(ID());
	idx_ = -1;
	o_ = 0;
}

void StringList::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	dav::gl::draw_rect(p.x, p.y, q.x, q.y, 0x303030);
	dav::gl::draw_rect(p.x + 1, p.y + 1, q.x, q.y, 0x909090);
	dav::gl::draw_rect(p.x + 2, p.y + 2, q.x - 1, q.y - 1, 0xc0c0c0);

	int x = p.x + 4, y = p.y + 2, i = o_;

	while(y < q.y - 2 - MXT_H)
	{
		if(i >= (long)choices_.size()) break;

		std::string s(choices_.at(i));

		if(s.length() * Text::C_W >= width() - Text::C_W / 2) s = s.substr(0, (width() - Text::C_W / 2) / Text::C_W);

		if(i == idx_ + o_)
		{
			dav::gl::draw_rect(x - 2, y, q.x - 2, y + MXT_H, 0x404040);
			Text::drawText(x, y + Text::C_W / 4, s, 0xffffff);
		}
		else
		{
			Text::drawText(x, y + Text::C_W / 4, s);
		}

		++i;
		y += MXT_H;
	}
}

void StringList::i_doMouseOver(bool over)
{
	if(!over)
	{
		idx_ = -1;
		dirty();
	}
}

void StringList::i_doMouseMove(int x, int y)
{
	int i = (y - 2) / MXT_H;

	if(i < 0 || i >= (long)choices_.size() - o_)
	{
		i = -1;
	}
	
	if(idx_ != i)
	{
		idx_ = i;
		dirty();
	}
}

void StringList::i_doMouseDown(MouseButtons b, int x, int y)
{

	if(b != MouseButtons::LEFT || x < 0 || x >= (int)width() || y < 0 || y >= (int)height()) return;

	int i = (y - 2) / MXT_H;

	if(i >= 0 && i < (long)choices_.size())
	{
		LOG("SL MouseDown %d,%d -> %d", x, y, i);
		activate_(choices_.at(i));
		hide();
	}
}

void StringList::i_doScroll(int d)
{
	LOG("SL Scrolling with %d", d);
	int oo = o_;

	if(d < 0)
	{
		if(o_ < (long)choices_.size() - (height() - 4) / MXT_H) ++o_;
	}
	else if(d > 0)
	{
		if(o_ > 0) --o_;
	}

	if(oo != o_) dirty();
}

}}

