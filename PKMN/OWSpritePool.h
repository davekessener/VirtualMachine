#ifndef OWSPRITEPOOL_H
#define OWSPRITEPOOL_H

#include <map>
#include "SDLImage.h"

#ifdef OWSPRITEPOOL_MAIN
#endif

class OWSpritePool
{
	public:
		static SDLImage *getSprite(int);
	private:
		static OWSpritePool& instance( );
		OWSpritePool( );
		~OWSpritePool( );
		SDLImage *_getSprite_(int);

		std::map<int, std::pair<SDLImage *, int>> sprites;

		static const int MAX_POOL_SIZE = 16;
};

#endif

