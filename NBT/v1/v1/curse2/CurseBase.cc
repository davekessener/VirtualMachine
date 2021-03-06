#define CURSE_BASE_MAIN
#include "CurseBase.h"
#undef CURSE_BASE_MAIN

namespace ncurses
{
	const int Attributes::UNDERSCORE = A_UNDERLINE;

	void Curse_base::initscr(void)
	{
		::initscr();
	}

	void Curse_base::endwin(void)
	{
		::endwin();
	}

	void Curse_base::refresh(void)
	{
		::refresh();
	}

	void Curse_base::keypad(void)
	{
		::keypad(stdscr, true);
	}

	void Curse_base::erase(void)
	{
		::werase(stdscr);
	}

	void Curse_base::chgat(int a, int c, int l)
	{
		::wchgat(stdscr, l, a, c, NULL);
	}

	void Curse_base::curs_set(int v)
	{
		::curs_set(v);
	}

	void Curse_base::getyx(int& y, int& x)
	{
		x = stdscr ? stdscr->_curx : -1;
		y = stdscr ? stdscr->_cury : -1;
	}

	void Curse_base::getmaxyx(int& y, int& x)
	{
		x = stdscr ? stdscr->_maxx + 1 : -1;
		y = stdscr ? stdscr->_maxy + 1 : -1;
	}

	void Curse_base::clrtoeol(void)
	{
		::wclrtoeol(stdscr);
	}

	void Curse_base::move(int y, int x)
	{
		::move(y, x);
	}

	void Curse_base::printw(const char *s, ...)
	{
		va_list l;
		va_start(l, s);

		::vwprintw(stdscr, s, l);

		va_end(l);
	}

	void Curse_base::vprintw(const char *s, va_list l)
	{
		::vwprintw(stdscr, s, l);
	}

	void Curse_base::vwprintw(const wchar_t *s, va_list l)
	{
		wchar_t buf[2048];
		vswprintf(buf, sizeof(buf) / sizeof(wchar_t), s, l);

		::waddnwstr(stdscr, buf, -1);
	}

	void Curse_base::addstr(const char *s)
	{
		::waddnstr(stdscr, s, -1);
	}

	void Curse_base::addwstr(const wchar_t *s)
	{
		::waddnwstr(stdscr, s, -1);
	}

	void Curse_base::cbreak(void)
	{
		::cbreak();
	}

	void Curse_base::nocbreak(void)
	{
		::nocbreak();
	}

	void Curse_base::echo(void)
	{
		::echo();
	}

	void Curse_base::noecho(void)
	{
		::noecho();
	}

	void Curse_base::raw(void)
	{
		::raw();
	}

	void Curse_base::noraw(void)
	{
		::noraw();
	}

	void Curse_base::timeout(int ms)
	{
		::wtimeout(stdscr, ms);
	}

	void Curse_base::getnstr(char *s, int n)
	{
		::wgetnstr(stdscr, s, n);
	}

	void Curse_base::getstr(char *s)
	{
		::wgetnstr(stdscr, s, -1);
	}

	void Curse_base::getnwstr(wchar_t *s, int n)
	{
		::wgetn_wstr(stdscr, reinterpret_cast<wint_t *>(s), n);
	}

	void Curse_base::getwstr(wchar_t *s)
	{
		::wget_wstr(stdscr, reinterpret_cast<wint_t *>(s));
	}

	void Curse_base::set_escdelay(int d)
	{
		::set_escdelay(d);
	}

	void Curse_base::clearRect(int x1, int y1, int x2, int y2)
	{
		char *s = static_cast<char *>(malloc(x2 - x1 + 1));
		memset(s, ' ', x2 - x1);
		s[x2 - x1] = '\0';

		for(int y = y1 ; y < y2 ; ++y)
		{
			setCursorPos(x1, y);
			addstr(s);
		}
	}

	int Curse_base::getch(void)
	{
		int ch = ERR, c = ERR;

		while((ch = ::wgetch(stdscr)) != ERR)
		{
			c = (c == ERR ? ch : ((c << 8) | ch));
		}

		switch(c)
		{
			case KEY_LEFT:
				c = Keys::LEFT;
				break;
			case KEY_RIGHT:
				c = Keys::RIGHT;
				break;
			case KEY_UP:
				c = Keys::UP;
				break;
			case KEY_DOWN:
				c = Keys::DOWN;
				break;
			case KEY_BACKSPACE:
			case 0x7f:
				c = Keys::BACKSPACE;
				break;
			case '\e':
				c = Keys::ESCAPE;
				break;
		}

		return c;
	}

	wchar_t Curse_base::getwch(void)
	{
		wchar_t ch;

		::wget_wch(stdscr, reinterpret_cast<wint_t *>(&ch));

		return ch;
	}
}

