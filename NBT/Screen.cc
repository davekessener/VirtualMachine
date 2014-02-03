#include "Screen.h"

Screen::Screen(std::ostream& os) : _os(os)
{
	getScreenSize(&_width, &_height);
}

Screen::Screen(int width, int height, std::ostream& os) : _width(width), _height(height), _os(os)
{
}

Screen::Screen(const Screen& screen) : _width(screen._width), _height(screen._height), _os(screen._os)
{
}

Screen::Screen(Screen&& screen) : _width(screen._width), _height(screen._height), _os(screen._os)
{
}

Screen::~Screen(void)
{
	resetColors();
//	clear();
	flush();
}

void Screen::clear(void) const
{
	_os << "\e[2J\e[1;1H";
}

void Screen::setPos(int x, int y) const
{
	_os << "\e[" << (y + 1) << ";" << (x + 1) << "H";
}

void Screen::setColor(int fgc, int bgc, int attr) const
{
	fgc += 30;
	if(bgc >= 0) bgc += 40;

	_os << "\e[" << fgc;
	if(bgc >= 0) _os << ";" << bgc;
	if(attr >= 0) _os << ";" << attr;
	_os << "m";
}

void Screen::resetColors(void) const
{
	_os << "\e[0m";
}

void Screen::put(int x, int y, char c) const
{
	setPos(x, y);
	_os << c;
}

void Screen::flush(void) const
{
	_os.flush();
}

int Screen::width(void) const
{
	return _width;
}

int Screen::height(void) const
{
	return _height;
}

