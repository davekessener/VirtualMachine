#ifndef OWSPRITERENDERER_H
#define OWSPRITERENDERER_H

#include "OWSprite.h"

#ifdef OWSPRITERENDERER_MAIN
#include "OWSpritePool.h"
#include "Screen.h"
#include <Misc/Logger.h>
#endif

class OWSpriteRenderer
{
	public:
		static void renderSpriteWithOffset(const OWSprite&, int, int);
};

#endif

