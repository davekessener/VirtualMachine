#ifndef OWSPRITE_H
#define OWSPRITE_H

#include "../common.h"
#include "Moving.h"

#ifdef OWSPRITE_MAIN
#endif

class OWSprite : public Moving
{
	public:
		OWSprite(int _i = 0, int _x = 0, int _y = 0) : Moving(_x, _y), index(_i), facing(0) { }
		~OWSprite( ) { }
		void walk(Direction);
		int getIndex( ) const { return index; }
		int getFacing( ) const { return facing; }
		int getSpriteOffset( ) const
			{ return facing * SIDE_FRAMES + 
						(((!dx && !dy) || (dx <= -8 || dx >= 8 || dy <= -8 || dy >= 8)) ? 0 : ((steps % 2) + 1)) + 
						(speed - 1) * SIDE_COUNT * SIDE_FRAMES; }
		static const int WIDTH = 16;
		static const int HEIGHT = 24;
	protected:
		static const int SIDE_COUNT = 4;
		static const int SIDE_FRAMES = 3;
	private:
		int index, facing;
};

#endif

