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
	template<typename TT>
		bool operator==(const point2d<TT>& p) { return x == p.x && y == p.y; }
	template<typename TT>
		bool operator!=(const point2d<TT>& p) { return x != p.x || y != p.y; }
	template<typename TT>
		point2d<T>& operator=(const point2d<TT>& p) { x = p.x; y = p.y; return *this; }
	T x, y;
};
typedef point2d<int> point;
typedef point vec;
typedef point2d<float> fpoint;

typedef std::vector<std::string> params_t;

typedef unsigned int uint;

#endif

