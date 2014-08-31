#include "Camera.h"
#include "Config.h"

namespace pkmn
{
	point Camera::getOffset(void) const
	{
		point p(pos_);

		if(lock_)
		{
			int d = Config::TILE_SIZE / 2;
			p = lock_->position();
			p.x -= d;
			p.y -= d;
		}

		return p;
	}
}

