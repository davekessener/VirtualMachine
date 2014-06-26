#include "Key.h"
#include <SDL2/SDL.h>

namespace vm { namespace sdl {

Key::Key(std::uint32_t v) : v_(v), c_(0xff), s_(SpecialKey::NOT)
{
	if(v_ < 256)
	{
		c_ = static_cast<char>(v_);
		if(c_ == '\r') c_ = '\n';
	}
	else switch(v_)
	{
#define MXT_KEY(id) case SDLK_##id: s_ = SpecialKey::id; break;
		MXT_KEY(LEFT);
		MXT_KEY(RIGHT);
		MXT_KEY(UP);
		MXT_KEY(DOWN);
#undef MXT_KEY
	}
}

Key::operator char(void) const
{
	return c_;
}

bool Key::isSpecial(void) const
{
	return s_ != SpecialKey::NOT;
}

SpecialKey Key::get(void) const
{
	return s_;
}

}}

