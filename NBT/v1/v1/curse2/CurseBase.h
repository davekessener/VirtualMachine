#ifndef CURSE_BASE_H
#define CURSE_BASE_H

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include "Curses.h"

#ifdef CURSE_BASE_MAIN
#include <ncurses.h>

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
#undef ERR
#endif

namespace ncurses
{
	class Curse_base
	{
		public:
			void start()
			{
				if(getenv("ESCDELAY") == NULL)
					set_escdelay(25);
				initscr();
				keypad();
			}
			void end() { endwin(); }
			void refresh();
			void clear() { erase(); }
			void clearRect(int, int, int, int);
			void showCursor(bool visible) { curs_set(visible ? 1 : 0); }
			void getCursorPos(int& x, int& y) { getyx(y, x); }
			int getCursorX() { int x, y; getyx(y, x); return x; }
			int getCursorY() { int x, y; getyx(y, x); return y; }
			void getScreenSize(int& w, int& h) { getmaxyx(h, w); }
			int getWidth() { int w, h; getmaxyx(h, w); return w; }
			int getHeight() { int w, h; getmaxyx(h, w); return h; }
			void clearLine()
				{ int x, y; getyx(y, x); move(y, 0); clrtoeol(); move(y, x); }
			void setCursorPos(int x, int y) { move(y, x); }
			void printf(const char *s, ...)
			{
				va_list l;
				va_start(l, s);

				vprintw(s, l);

				va_end(l);
			}
			void wprintf(const wchar_t *s, ...)
			{
				va_list l;
				va_start(l, s);

				vwprintw(s, l);

				va_end(l);
			}
			int getChar() { return getch(); }
			void setCBreak(bool set) { if(set) cbreak(); else nocbreak(); }
			void setEcho(bool set) { if(set) echo(); else noecho(); }
			void setTimeout(int t) { timeout(t); }
			void changeAttribute(int a, int c = 0, int l = 1) { chgat(a, c, l); }

			static const int ERR = (-1);
		private:
			void initscr( );
			void endwin( );
			void keypad( );
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
	};
}

#endif

