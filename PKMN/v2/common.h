#ifndef PKMN_COMMON_H
#define PKMN_COMMON_H

#include <dav/inc.h>
#include <string>
#include <vector>

template<typename T>
struct point2d
{
	point2d( ) : x(0), y(0) { }
	point2d(const T& x_, const T& y_) : x(x_), y(y_) { }
	T x, y;
};
typedef point2d<int> point;
typedef point vec;
typedef point2d<float> fpoint;

typedef std::vector<std::string> params_t;

typedef unsigned int uint;

#endif

