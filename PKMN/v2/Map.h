#ifndef PKMN_MAP_H
#define PKMN_MAP_H

#include "common.h"
#include "abstract/Manager.h"
#include "render/TSHelper.h"

namespace pkmn
{
	class Map
	{
		public:
			Map(DWORD);
			void render(int, int) const;
		private:
			const abstract::Map &map_;
			render::TileSet ts_;
	};
}

#endif

