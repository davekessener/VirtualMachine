#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <SDL2/SDL.h>
#include <string>
#include <cstdlib>
#include <cstdint>
#include "Exception.h"

#define SDLException(...) Exception(::format("[%s:%d] ",__FILE__, __LINE__)+::format(__VA_ARGS__))

const int TILE_SIZE = 16;
const std::string FE = ".png";

typedef std::uint32_t button_t;
namespace Button
{
	enum
	{
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		MIDDLE = 4,
		SHIFT = 8,
		CTRL = 16,
		ALT = 32,
		WIN = 64,
		FN = 128
	};
}

struct Rect
{
	Rect(int _x = 0, int _y = 0, int _w = 0, int _h = 0) : x(_x), y(_y), w(_w), h(_h) { }
	operator SDL_Rect( ) { SDL_Rect r = {x, y, w, h}; return r; }
	int x, y, w, h;
};

struct Point
{
	Point(int _x, int _y) : x(_x), y(_y) { }
	operator SDL_Point( ) { SDL_Point p = {x, y}; return p; }
	int x, y;
};

std::string format(const char * = NULL, ...);

#endif

