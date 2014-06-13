#include "Keyboard.h"
#include "Screen.h"
#include "Logger.h"
#include "CPU.h"

namespace vm { namespace periphery {

using sdl::Screen;

BasicKeyboard::BasicKeyboard(void) : enabled_(false)
{
}

void BasicKeyboard::in(WORD v)
{
	switch(v)
	{
		case 0x00:
			enabled_ = false;
			break;
		case 0x01:
			enabled_ = true;
			break;
		default:
			LOG("Unrecognized command [0x%04x]", v);
			break;
	}
}

WORD BasicKeyboard::out(void)
{
	if(enabled_)
	{
		if(Screen::instance().keyPressed())
		{
			return static_cast<char>(Screen::instance().keyPoll());
		}
	}

	return 0;
}

void BasicKeyboard::step(void)
{
	if(enabled_)
	{
		if(Screen::instance().keyPressed())
		{
			cpu_->interrupt(INT_KEYBOARD);
		}
	}
	else while(Screen::instance().keyPressed())
	{
		Screen::instance().keyPoll();
	}
}

}}

