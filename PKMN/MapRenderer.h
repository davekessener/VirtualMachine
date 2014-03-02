#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <map>
#include <vector>
#include <cstdint>
#include "SDLImage.h"
#include "Map.h"
#include "Pool.h"

#ifdef MAPRENDERER_MAIN
#include "Logger.h"
#include "Timer.h"

#define MR_POOL_MAXSIZE 8
#endif

class MapRenderer : public Pool<

class MapRenderer
{
	typedef std::vector<std::uint32_t> layer_t;

	public:
		static void init( );
		static void quit( );
		static MapRenderer *getRenderer(const Map&);
		static void renderMap(const Map&, int);
	private:
		MapRenderer(int, int, const layer_t&, const layer_t&, const layer_t&);
		~MapRenderer( );
		static std::map<int, std::pair<MapRenderer *, int>> pool;
		static SDLImage tileset;
		static void prerenderLayer(int, int, SDLImage&, const layer_t&);

		SDLImage map_bottom, map_top;
		const layer_t animation;
		int width, height;
};

#endif

