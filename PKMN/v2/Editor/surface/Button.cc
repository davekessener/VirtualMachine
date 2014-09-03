#include "Button.h"
#include "../Text.h"
#include "ButtonHelper.h"
#include <dav/gl.h>
#include <dav/Logger.h>

namespace editor { namespace surface {

void Button::i_doRender(void) const
{
	using dav::gl::draw_rect;

	point p(getAbsCoords()), q(p.x + width(), p.y + height());
	point t((p.x + q.x) / 2 - msg_.length() * Text::C_W / 2, (p.y + q.y) / 2 - Text::C_W / 2);

	ButtonHelper::renderButton(p.x, p.y, q.x, q.y, alt_);

	if(alt_)
	{
//		draw_rect(p.x, p.y, q.x, q.y, 0xf2f2f2);
//		draw_rect(p.x, p.y, q.x - 1, q.y - 1, 0x303030);
//		draw_rect(p.x + 1, p.y + 1, q.x - 1, q.y - 1, 0xc0c0c0);
//		draw_rect(p.x + 1, p.y + 1, q.x - 2, q.y - 2, 0x909090);
//		draw_rect(p.x + 2, p.y + 2, q.x - 1, q.y - 1, 0xc0c0c0);

		Text::drawText(t.x + 1, t.y + 1, msg_);
	}
	else
	{
//		draw_rect(p.x, p.y, q.x, q.y, 0x303030);
//		draw_rect(p.x, p.y, q.x - 1, q.y - 1, 0xf2f2f2);
//		draw_rect(p.x + 1, p.y + 1, q.x - 1, q.y - 1, 0x909090);
//		draw_rect(p.x + 1, p.y + 1, q.x - 2, q.y - 2, 0xc0c0c0);

		Text::drawText(t.x, t.y, msg_);
	}
}

void Button::i_doMouseDown(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && isOver(x, y))
	{
		down_ = true;
		changeAlt(true);
	}
}

void Button::i_doMouseMove(int x, int y)
{
	if(down_)
	{
		changeAlt(isOver(x, y));
	}
}

void Button::i_doMouseUp(MouseButtons b, int x, int y)
{
	if(b == MouseButtons::LEFT && isOver(x, y))
	{
		fn_();
	}

	down_ = false;
	changeAlt(false);
}

}}

