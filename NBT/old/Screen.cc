#include "Screen.h"

Screen::Screen(std::ostream& os) : _os(os), buffer("", std::ios::app | std::ios::out)
{
	getScreenSize(&_width, &_height);
}

Screen::Screen(int width, int height, std::ostream& os) : _width(width), _height(height), _os(os), buffer("", std::ios::app | std::ios::out)
{
}

Screen::Screen(const Screen& screen) : _width(screen._width), _height(screen._height), _os(screen._os), buffer("", std::ios::app | std::ios::out)
{
}

Screen::Screen(Screen&& screen) : _width(screen._width), _height(screen._height), _os(screen._os), buffer("", std::ios::app | std::ios::out)
{
}

Screen::~Screen(void)
{
	resetColors();
//	clear();
	setPos(0, _height - 1);
	buffer << "\n\r";
	flush();
}

void Screen::clear(void)
{
	buffer << "\e[2J\e[1;1H";
}

void Screen::setPos(int x, int y)
{
	buffer << "\e[" << (y + 1) << ";" << (x + 1) << "H";
}

void Screen::setColor(int fgc, int bgc, int attr)
{
	fgc += 30;
	if(bgc >= 0) bgc += 40;

	buffer << "\e[" << fgc;
	if(bgc >= 0) buffer << ";" << bgc;
	if(attr >= 0) buffer << ";" << attr;
	buffer << "m";
}

void Screen::resetColors(void)
{
	buffer << "\e[0m";
}

void Screen::put(int x, int y, char c)
{
	setPos(x, y);
	buffer << c;
}

void Screen::flush(void)
{
	if(needsRedraw())
	{
		_os << buffer.str();
		_os.flush();
		buffer.clear();
	}
}

int Screen::width(void)
{
	return _width;
}

int Screen::height(void)
{
	return _height;
}

bool Screen::needsRedraw(void)
{
	return buffer.tellp() ? true : false;
}

void Screen::eraseLine(void)
{
	buffer << "\e[2K";
}

