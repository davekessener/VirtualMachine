#include <ncurses.h>
#include "Screen.h"

void Screen::clear(void)
{
	::clear();
}

void Screen::setPos(int x, int y)
{
	move(y, x);
}

void Screen::flush(void)
{
	refresh();
}

int Screen::width(void)
{
	int w, h;
	
	getmaxyx(stdscr, h, w);

	return w;
}

int Screen::height(void)
{
	int w, h;
	
	getmaxyx(stdscr, h, w);

	return h;
}

void Screen::eraseLine(void)
{
	int x, y;
	getyx(stdscr, y, x);
	move(y, 0);
	clrtoeol();
	move(y, x);
}

void Screen::puts(const wchar_t *ws)
{
	addwstr(ws);
}

void Screen::puts(const char *s)
{
	addstr(s);
}

Screen& Screen::instance(void)
{
	static Screen s;
	return s;
}

Screen::Screen(void)
{
	setlocale(LC_ALL, "");
	initscr();
}

Screen::~Screen(void)
{
	endwin();
}

