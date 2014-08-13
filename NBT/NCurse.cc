#include "NCurse.h"

#include <string>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

#define WPRINT_BUFSIZE 2048

#undef getyx
#undef getmaxyx
#undef clrtoeol
#undef getch
#undef getstr
#undef getnstr
#undef addwstr
#undef addstr
#undef timeout
#undef erase
#undef chgat

namespace ncurses
{
	void initscr(void)
	{
		::initscr();
	}
	
	void endwin(void)
	{
		::endwin();
	}
	
	void refresh(void)
	{
		::refresh();
	}
	
	void keypad(void)
	{
		::keypad(stdscr, true);
	}

	void erase(void)
	{
		::werase(stdscr);
	}
	
	void chgat(int a, int c, int l)
	{
		::wchgat(stdscr, l, a, c, NULL);
	}
	
	void curs_set(int v)
	{
		::curs_set(v);
	}
	
	void getyx(int& y, int& x)
	{
		x = stdscr ? stdscr->_curx : -1;
		y = stdscr ? stdscr->_cury : -1;
	}
	
	void getmaxyx(int& y, int& x)
	{
		x = stdscr ? stdscr->_maxx + 1 : -1;
		y = stdscr ? stdscr->_maxy + 1 : -1;
	}
	
	void clrtoeol(void)
	{
		::wclrtoeol(stdscr);
	}
	
	void move(int y, int x)
	{
		::move(y, x);
	}
	
	void printw(const char *s, ...)
	{
		va_list l;
		va_start(l, s);
	
		::vwprintw(stdscr, s, l);
	
		va_end(l);
	}
	
	void vprintw(const char *s, va_list l)
	{
		::vwprintw(stdscr, s, l);
	}
	
	void vwprintw(const wchar_t *s, va_list l)
	{
		wchar_t buf[WPRINT_BUFSIZE];
		vswprintf(buf, sizeof(buf) / sizeof(wchar_t), s, l);
	
		::waddnwstr(stdscr, buf, -1);
	}
	
	void addstr(const char *s)
	{
		::waddnstr(stdscr, s, -1);
	}
	
	void addwstr(const wchar_t *s)
	{
		::waddnwstr(stdscr, s, -1);
	}
	
	void cbreak(void)
	{
		::cbreak();
	}
	
	void nocbreak(void)
	{
		::nocbreak();
	}
	
	void echo(void)
	{
		::echo();
	}
	
	void noecho(void)
	{
		::noecho();
	}
	
	void raw(void)
	{
		::raw();
	}
	
	void noraw(void)
	{
		::noraw();
	}
	
	void timeout(int ms)
	{
		::wtimeout(stdscr, ms);
	}
	
	void getnstr(char *s, int n)
	{
		::wgetnstr(stdscr, s, n);
	}
	
	void getstr(char *s)
	{
		::wgetnstr(stdscr, s, -1);
	}
	
	void getnwstr(wchar_t *s, int n)
	{
		::wgetn_wstr(stdscr, reinterpret_cast<wint_t *>(s), n);
	}
	
	void getwstr(wchar_t *s)
	{
		::wget_wstr(stdscr, reinterpret_cast<wint_t *>(s));
	}
	
	void set_escdelay(int d)
	{
		::set_escdelay(d);
	}
	
	void clearRect(int x1, int y1, int x2, int y2)
	{
		std::string s(x2 - x1, ' ');
	
		for(int y = y1 ; y < y2 ; ++y)
		{
			move(y, x1);
			addstr(s.c_str());
		}
	}
	
	int getch(void)
	{
		int ch = ERR, c = ERR;
	
		while((ch = ::wgetch(stdscr)) != ERR)
		{
			c = (c == ERR ? ch : ((c << 8) | ch));
		}
	
		switch(c)
		{
			case KEY_LEFT:
				c = NKEY_LEFT;
				break;
			case KEY_RIGHT:
				c = NKEY_RIGHT;
				break;
			case KEY_UP:
				c = NKEY_UP;
				break;
			case KEY_DOWN:
				c = NKEY_DOWN;
				break;
			case KEY_BACKSPACE:
			case 0x7f:
				c = NKEY_BACKSPACE;
				break;
			case '\e':
				c = NKEY_ESCAPE;
				break;
		}
	
		return c;
	}
	
	wchar_t getwch(void)
	{
		wchar_t ch;
	
		::wget_wch(stdscr, reinterpret_cast<wint_t *>(&ch));
	
		return ch;
	}
}

