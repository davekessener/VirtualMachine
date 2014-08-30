#ifndef PKMN_COMMON_H
#define PKMN_COMMON_H

#include <dav/inc.h>

struct point
{
	point( ) : x(0), y(0) { }
	point(int x_, int y_) : x(x_), y(y_) { }
	int x, y;
};

typedef unsigned int uint;

#endif

