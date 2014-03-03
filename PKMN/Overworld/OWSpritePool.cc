#define OWSPRITEPOOL_MAIN
#include "OWSpritePool.h"
#undef OWSPRITEPOOL_MAIN

SDLImage *OWSpritePool::getSprite(int id)
{
	SDLImage *r = pool::instance().getFromPool(id);

	if(!r->opened())
	{
		std::ostringstream ss;
		ss << "./sprites/" << id << ".png";
		r->open(ss.str());
	}

	return r;
}

