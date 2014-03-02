#define OWSPRITE_MAIN
#include "OWSprite.h"
#undef OWSPRITE_MAIN

OWSprite::OWSprite(int _i, int _x, int _y) : index(_i), x(_x), y(_y), facing(0), dx(0), dy(0), speed(1)
{
}

OWSprite::~OWSprite(void)
{
}

void OWSprite::update(void)
{
}

void OWSprite::walk(Direction d)
{
}

