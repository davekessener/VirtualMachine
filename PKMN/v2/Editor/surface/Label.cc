#include "Label.h"
#include "../Text.h"
#include <dav/gl.h>

namespace editor { namespace surface {

uint Label::getEstWidth(void) const
{
	return msg_.length() * Text::C_W;
}

void Label::i_doInit(void)
{
	uint l = width() / Text::C_W;
	if(msg_.length() > l) msg_ = msg_.substr(0, l);
}

void Label::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	Text::drawText(p.x, (p.y + q.y) / 2 - Text::C_W / 2, msg_);
}

}}

