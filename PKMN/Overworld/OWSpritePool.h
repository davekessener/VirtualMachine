#ifndef OWSPRITEPOOL_H
#define OWSPRITEPOOL_H

#include <map>
#include "SDLImage.h"
#include "Pool.h"
#include "Singleton.h"

#ifdef OWSPRITEPOOL_MAIN
#include <sstream>

#define OWS_POOL_MAXSIZE 24

template<typename T> class _pool_validation_age : public PoolValidationAge<T, OWS_POOL_MAXSIZE> { };
#else
template<typename T>
class _pool_validation_age;
#endif


class OWSpritePool
{
	typedef Singleton<Pool<PoolRetainerNew<SDLImage>, int, _pool_validation_age>> pool;
	
	public:
		static SDLImage *getSprite(int);
	private:
		OWSpritePool( ) { }
		OWSpritePool(const OWSpritePool&);
		OWSpritePool& operator=(const OWSpritePool&);
		~OWSpritePool( ) { }
};

#endif

