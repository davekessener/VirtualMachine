#include "Dialog.h"
#include "Button.h"
#include "../Text.h"
#include <dav/gl.h>

#define MXT_B 4

namespace editor { namespace surface {

void Dialog::i_doInit(void)
{
	Surface_ptr p(new Button("OK", std::bind(&Dialog::hide, this)));
	p->init(width() / 3, height() * 3 / 4, width() / 3, Text::C_W * 3 / 2);
	addChild(p);
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

