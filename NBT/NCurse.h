#ifndef NCURSE_H
#define NCURSE_H

#include <cstdint>

#ifdef NCURSE_MAIN
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
#endif

namespace ncurses
{
	void initscr( );
	void endwin( );
	void keypad( );
	void refresh( );
	void erase( );
	void chgat(int, int, int);
	void curs_set(int);
	void getyx(int&, int&);
	void getmaxyx(int&, int&);
	void clrtoeol( );
	void move(int, int);
	void printw(const char *, ...);
	void vprintw(const char *, va_list);
	void vwprintw(const wchar_t *, va_list);
	void addstr(const char *);
	void addwstr(const wchar_t *);
	int getch( );
	wchar_t getwch( );
	void cbreak( );
	void nocbreak( );
	void echo( );
	void noecho( );
	void raw( );
	void noraw( );
	void timeout(int);
	void getnstr(char *, int);
	void getstr(char *);
	void getnwstr(wchar_t *, int);
	void getwstr(wchar_t *);
	void set_escdelay(int);

	enum
	{
		NKEY_LEFT,
		NKEY_RIGHT,
		NKEY_UP,
		NKEY_DOWN,
		NKEY_BACKSPACE,
		NKEY_ESCAPE
	};
}

#endif

