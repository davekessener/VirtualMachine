#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <SDL2/SDL.h>
#include <string>
#include <cstdlib>
#include "Exception.h"

#define min(i,j) ((i)<(j)?(i):(j))
#define max(i,j) ((i)>(j)?(i):(j))
#define SDLException(...) Exception(::format("[%s:%d] ",__FILE__, __LINE__)+::format(__VA_ARGS__))

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

