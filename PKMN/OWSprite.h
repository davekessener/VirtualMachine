#ifndef OWSPRITE_H
#define OWSPRITE_H

#include "common.h"

#ifdef OWSPRITE_MAIN
#endif

class OWSprite
{
	public:
		OWSprite(int = 0, int = 0, int = 0);
		~OWSprite( );
		void update( );
		int getIndex( ) const { return index; }
		int getFacing( ) const { return facing; }
		int getX( ) const { return x; }
		int getY( ) const { return y; }
		int getDX( ) const { return dx; }
		int getDY( ) const { return dy; }
		int getSpeed( ) const { return speed; }
		int getSpriteOffset( ) const { return facing * SIDE_FRAMES + (dx == 0 ? 0 : (dx < 8 && dx > -8 ? 1 : 2)) + (speed - 1) * SIDE_COUNT * SIDE_FRAMES; }
		void walk(Direction);
	protected:
		static const int SIDE_COUNT = 4;
		static const int SIDE_FRAMES = 3;
	private:
		int index, facing;
		int x, y, dx, dy;
		int speed;
};

#endif

