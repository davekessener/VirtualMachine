#ifndef OWSPRITEPOOL_H
#define OWSPRITEPOOL_H

#include <map>
#include <Misc/SDLImage.h>
#include "Pool.h"
#include "Singleton.h"
#include "Screen.h"

#ifdef OWSPRITEPOOL_MAIN
#include <sstream>

#define OWS_POOL_MAXSIZE 24

template<typename T>
class _pool_validation_age  : public PoolCounted<T, OWS_POOL_MAXSIZE>
{
	public:
		typedef typename PoolSimple<T>::return_type return_type;

		_pool_validation_age( ) { }
		_pool_validation_age(return_type v) : PoolSimple<T>(v), PoolCounted<T, OWS_POOL_MAXSIZE>(v) { }
};
#else
template<typename T>
class _pool_validation_age;
#endif


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

