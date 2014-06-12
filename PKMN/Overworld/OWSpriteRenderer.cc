#define OWSPRITERENDERER_MAIN
#include "OWSpriteRenderer.h"
#undef OWSPRITERENDERER_MAIN

void OWSpriteRenderer::renderSpriteWithOffset(const OWSprite& sprite, int dx, int dy)
{
	SDLImage *tex = OWSpritePool::getSprite(sprite.getIndex());

	SDL_Rect ro, rt;

	ro.w = rt.w = OWSprite::WIDTH;
	ro.h = rt.h = OWSprite::HEIGHT;

	ro.x = sprite.getSpriteOffset() * OWSprite::WIDTH;
	ro.y = 0;

	rt.x = -dx + sprite.getX() * 16 + sprite.getDX();
	rt.y = -dy + sprite.getY() * 16 + sprite.getDY() - (OWSprite::HEIGHT - 16);

	rt.x += (Screen::SCREEN_WIDTH / 2 - OWSprite::WIDTH / 2) & ~0x0f;
	rt.y += (Screen::SCREEN_HEIGHT / 2 - OWSprite::HEIGHT / 2) & ~0x0f;

	if(rt.x <= -OWSprite::WIDTH || rt.y <= -OWSprite::HEIGHT
			|| rt.x > Screen::SCREEN_WIDTH || rt.y > Screen::SCREEN_HEIGHT)
	{
		return;
	}

	Screen::instance().toScreen(static_cast<SDL_Texture *>(*tex), ro, rt);
}

