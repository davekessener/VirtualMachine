#include "Sprite.h"
#include <dav/Logger.h>
#include "../Config.h"
#include "SHelper.h"

namespace pkmn { namespace render {

Sprite::Sprite(DWORD id) : id_(id)
{
	SHelper::registerSprite(id_);
}

Sprite::~Sprite(void) noexcept
{
	SHelper::freeSprite(id_);
}

void Sprite::render(Direction d, float f, int x, int y) const
{
	uint phase =   static_cast<uint>(d) * Config::SPRITE_PHASES
				 + (f > 0 && f < 0.5 ? 1
				 					 : (f < 0 && f > -0.5 ? 2 : 0));

	SHelper::render(id_, phase, x, y);
}

}}

