#define OWSPRITEPOOL_MAIN
#include "OWSpritePool.h"
#undef OWSPRITEPOOL_MAIN

SDLImage *OWSpritePool::getSprite(int idx)
{
	return instance()._getSprite_(idx);
}

OWSpritePool& OWSpritePool::instance(void)
{
	static OWSpritePool _p;
	return _p;
}

OWSpritePool::OWSpritePool(void)
{
}

OWSpritePool::~OWSpritePool(void)
{
	for(auto i = sprites.begin() ; i != sprites.end() ; ++i)
	{
		delete i->second.first;
	}
	sprites.clear();
}

SDLImage *OWSpritePool::_getSprite_(int idx)
{
	if(sprites.count(idx) > 0)
	{
	}
	else
	{
	}

	return NULL;
}

