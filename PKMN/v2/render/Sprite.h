#ifndef PKMN_RENDER_SPRITE_H
#define PKMN_RENDER_SPRITE_H

#include "../common.h"
#include "../Direction.h"

namespace pkmn
{
	namespace render
	{
		class Sprite
		{
			public:
				Sprite(DWORD);
				~Sprite( ) noexcept;
				inline DWORD ID( ) const { return id_; }
				void render(Direction, float, int, int) const;
			private:
				DWORD id_;
		};
	}
}

#endif

