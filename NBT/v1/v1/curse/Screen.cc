#define CURSE_SCREEN_MAIN
#include "Screen.h"
#undef CURSE_SCREEN_MAIN

namespace ncurses
{
	void Screen::printf(const char *s, ...)
	{
		va_list l;
		va_start(l, s);

		if(curse.isRunning())
		{
			curse.vprintf(s, l);
		}
		else
		{
			vprintf(s, l);
		}

		va_end(l);
	}

	void Screen::printf(const wchar_t *s, ...)
	{
		va_list l;
		va_start(l, s);

		if(curse.isRunning())
		{
			curse.vprintfw(s, l);
		}
		else
		{
			vwprintf(s, l);
		}

		va_end(l);
	}

	void Screen::refresh(void)
	{
		if(curse.isRunning())
		{
			curse.flush();
		}
#ifdef DEBUG
		else assert(false);
#endif
	}

	void Screen::move(int x, int y)
	{
		if(curse.isRunning())
		{
			curse.moveCursor(x, y);
		}
#ifdef DEBUG
		else assert(false);
#endif
	}

	void Screen::eraseLine(int y)
	{
#ifdef DEBUG
		assert(curse.isRunning());
#endif

		Curse::Position pos = curse.getCursorPosition();

		move(0, y >= 0 ? y : pos.y);

		curse.eraseToEndOfLine();

		move(pos.x, pos.y);
	}

	int Screen::getCursorX(void)
	{
		if(curse.isRunning())
		{
			return curse.getCursorPosition().x;
		}
#ifdef DEBUG
		else assert(false);
#endif

		return -1;
	}

	int Screen::getCursorY(void)
	{
		if(curse.isRunning())
		{
			return curse.getCursorPosition().y;
		}
#ifdef DEBUG
		else assert(false);
#endif

		return -1;
	}

	int Screen::getScreenWidth(void)
	{
		if(curse.isRunning())
		{
			return curse.getScreenSize().width;
		}
#ifdef DEBUG
		else assert(false);
#endif

		return -1;
	}

	int Screen::getScreenHeight(void)
	{
		if(curse.isRunning())
		{
			return curse.getScreenSize().height;
		}
#ifdef DEBUG
		else assert(false);
#endif

		return -1;
	}

	Screen& Screen::instance(void)
	{
		static Screen s;
		return s;
	}

	Screen::Screen(void) : curse(Curse::instance())
	{
	}

	Screen::~Screen(void)
	{
	}
}

