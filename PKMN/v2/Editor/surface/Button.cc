#include "Button.h"
#include "../Text.h"
#include "ButtonHelper.h"
#include "IconBuffer.h"
#include <dav/gl.h>
#include <dav/Logger.h>

namespace editor { namespace surface {

void Button::i_doInit(void)
{
	down_ = false;
	alt_ = false;
	force_ = false;
}

void Button::i_doRender(void) const
{
	using dav::gl::fill_rect;

	bool d = alt_ || force_;

	point p(getAbsCoords()), q(p.x + width(), p.y + height());
	point t((p.x + q.x) / 2 - msg_.length() * Text::C_W / 2 + d, (p.y + q.y) / 2 - Text::C_W / 2 + d);
	point i((p.x + q.x) / 2 - 8 + d, (p.y + q.y) / 2 - 8 + d);

	ButtonHelper::renderButton(p.x, p.y, q.x, q.y, d);
	
	if(id_)
	{
		IconBuffer::renderIcon(id_, i.x, i.y, i.x + 16, i.y + 16);
	}
	else if(!msg_.empty())
	{
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

