#include "Dialog.h"
#include "Button.h"
#include "../Text.h"
#include <dav/gl.h>

#define MXT_B 4

namespace editor { namespace surface {

Dialog::Dialog(const std::string& msg, std::initializer_list<btn_t> l, const std::string& cancel) : msg_(msg)
{
	btns_.reserve(l.size() + 1);

	for(const btn_t& b : l)
	{
		btns_.push_back(Surface_ptr(new Button(b.first, [this, b](void) { b.second(); hide(); })));
	}

	btns_.push_back(Surface_ptr(new Button(cancel, std::bind(&Dialog::hide, this))));
}

void Dialog::i_doInit(void)
{
	int n = btns_.size(), cw = Text::C_W / 2;
	point s((width() - 4 * Text::C_W) / n - cw, Text::C_W * 2), 
		  o(width() / 2 - n / 2 * (s.x + cw) + (n % 2 ? -s.x / 2 : cw / 2), height() * 3 / 4);

	for(Surface_ptr p : btns_)
	{
		p->init(o.x, o.y, s.x, s.y);
		addChild(p);
		o.x += s.x + cw;
	}
}

void Dialog::i_doUpdate(int d)
{
}

void Dialog::i_doRender(void) const
{
	point p(getAbsCoords());
	dav::gl::draw_rect(p.x, p.y, p.x + width(), p.y + height(), 0x333333);
	dav::gl::draw_rect(p.x + MXT_B, p.y + MXT_B, p.x + width() - MXT_B, p.y + height() - MXT_B, 0xdddddd);
	Text::drawText(p.x + (width() - msg_.length() * Text::C_W) / 2, p.y + height() / 3 - Text::C_W / 2, msg_);
}

}}

