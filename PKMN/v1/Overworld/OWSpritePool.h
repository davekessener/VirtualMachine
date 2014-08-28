#ifndef OWSPRITEPOOL_H
#define OWSPRITEPOOL_H

#include <map>
#include "../Misc/SDLImage.h"
#include "../Pool.h"
#include "../Singleton.h"
#include "../Screen.h"

template<typename T>
class _pool_validation_age;

class OWSpritePool
{
	typedef Singleton<Pool<SDLImage, int, _pool_validation_age>> pool;
	
	public:
		static SDLImage *getSprite(int);
	private:
		OWSpritePool( ) { }
		OWSpritePool(const OWSpritePool&);
		OWSpritePool& operator=(const OWSpritePool&);
		~OWSpritePool( ) { }
};

#endif

