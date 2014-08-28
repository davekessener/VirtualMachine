#include <cstdlib>
#include <locale.h>
#include "Curse.h"
#include "NCurse.h"

namespace ncurses
{
	Curse::Curse(void)
	{
	}

	Curse::~Curse(void)
	{
	}

	void Curse::init(void)
	{
		if(getenv("ESCDELAY") == NULL)
		{
			set_escdelay(25);
		}
		setlocale(LC_ALL, "");
		initscr();
		keypad();
		cbreak();
		timeout(0);
	}

	void Curse::finalize(void)
	{
		endwin();
	}

	void Curse::flush(void)
	{
		refresh();
	}

	void Curse::getCursorPos(int& x, int& y)
	{
		getyx(y, x);
	}

	void Curse::setCursorPos(int x, int y)
	{
		move(y, x);
	}

	void Curse::getScreenSize(int& w, int& h)
	{
		getmaxyx(h, w);
	}

	void Curse::showCursor(bool show)
	{
		curs_set(show ? 1 : 0);
	}

	void Curse::eraseScreen(void)
	{
		erase();
	}

	void Curse::eraseLine(void)
	{
		int x, y;
		getyx(y, x);
		move(y, 0);
		clrtoeol();
		move(y, x);
	}

	int  Curse::getChar(void)
	{
		int c = getch();

		switch(c)
		{
			case NKEY_LEFT:
				c = display::Keys::LEFT;
				break;
			case NKEY_RIGHT:
				c = display::Keys::RIGHT;
				break;
			case NKEY_UP:
				c = display::Keys::UP;
				break;
			case NKEY_DOWN:
				c = display::Keys::DOWN;
				break;
			case NKEY_BACKSPACE:
			case 0x7f:
				c = display::Keys::BACKSPACE;
				break;
			case NKEY_ESCAPE:
			case '\e':
				c = display::Keys::ESCAPE;
				break;
		}

		return c;
	}

	void Curse::pause(void)
	{
		while(getch() != (-1));
		while(getch() == (-1));
	}

	void Curse::printf(const char *s, va_list l)
	{
		vprintw(s, l);
	}

	void Curse::printfw(const wchar_t *s, va_list l)
	{
		vwprintw(s, l);
	}

	void Curse::saveCursorPos(void)
	{
		int x, y;
		getyx(y, x);

		cursorBackup.push_back(std::make_pair(x, y));
	}

	void Curse::restoreCursorPos(void)
	{
		if(!cursorBackup.empty())
		{
			std::pair<int, int> p = cursorBackup.at(0);

			move(p.second, p.first);

			cursorBackup.pop_front();
		}
	}
}

