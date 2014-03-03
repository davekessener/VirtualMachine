#ifndef MOVING_H
#define MOVING_H

#include "common.h"

class Moving
{
	public:
		Moving(int _x = 0, int _y = 0) : x(_x), y(_y), dx(0), dy(0), speed(1), steps(0) { }
		virtual ~Moving( ) { }
		virtual void update( );
		virtual void walk(Direction);
		int getX( ) const { return x; }
		int getY( ) const { return y; }
		int getDX( ) const { return dx; }
		int getDY( ) const { return dy; }
		int getHDist( ) const { return x * 16 + dx; }
		int getVDist( ) const { return y * 16 + dy; }
		int getSpeed( ) const { return speed; }
		bool isMoving( ) { return dx || dy; }
	protected:
		int x, y, dx, dy;
		int speed, steps;
};

#endif

