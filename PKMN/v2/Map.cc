#include "Map.h"
#include "Config.h"
#include <dav/Logger.h>

namespace pkmn
{
	Map::Map(DWORD id) : map_(abstract::Manager::getMap(id))
	{
		std::set<DWORD> tsm;

		for(size_t i = 0 ; i < map_.size() ; ++i)
		{
			for(const DWORD& v : map_[i])
			{
				tsm.insert(v);
			}
		}

		ts_ = render::TSHelper::generate(tsm);

		LOG("New map created.");
		LOG("> %lu tiles in %u x %u map", tsm.size(), map_.width(), map_.height());
	}

	void Map::render(int tx, int ty) const
	{
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
		long w = map_.width(), h = map_.height();
		long d = Config::TILE_SIZE;
		long sx = max(0, -tx/d), sy = max(0, -ty/d);
		long dw = min(w, (Config::SCREEN_WIDTH - tx) / d + 1);
		long dh = min(h, (Config::SCREEN_HEIGHT - ty) / d + 1);
#undef max
#undef min

		for(size_t l = 0 ; l < map_.size() ; ++l)
		{
			if(map_.isLayerEmpty(l)) continue;

			const auto& layer(map_[l]);

			for(long y = sy ; y < dh ; ++y)
			{
				for(long x = sx ; x < dw ; ++x)
				{
					DWORD v = layer[x + y * w];

					if(v)
					{
						ts_.render(tx + x * d, ty + y * d, v);
					}
				}
			}
		}
	}
}

