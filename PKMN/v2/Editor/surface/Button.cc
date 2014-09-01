#include "Button.h"
#include "../Text.h"
#include <dav/gl.h>
#include <dav/Logger.h>

namespace editor { namespace surface {

void Button::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	dav::gl::draw_rect(p.x, p.y, q.x, q.y, 0x333333);
	dav::gl::draw_rect(p.x, p.y, q.x - 1, q.y - 1, 0xf2f2f2);
	dav::gl::draw_rect(p.x + 1, p.y + 1, q.x - 1, q.y - 1, 0x999999);
	dav::gl::draw_rect(p.x + 1, p.y + 1, q.x - 2, q.y - 2, 0xcccccc);

	Text::drawText((p.x + q.x) / 2 - msg_.length() * Text::C_W / 2, (p.y + q.y) / 2 - Text::C_W / 2, msg_);
}

void Button::i_doMouseUp(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && x >= 0 && x < (int)width() && y >= 0 && y < (int)height())
	{
		LOG("Button pressed!");
		fn_();
	}
}

}}

