#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <map>
#include <vector>
#include <cstdint>
#include "../Misc/SDLImage.h"
#include "Map.h"
#include "../Pool.h"
#include "../Singleton.h"
#include "../Screen.h"

class MapRenderer
{
	typedef std::vector<std::uint32_t> layer_t;

	public:
		static MapRenderer *getRenderer(const Map&);
		static void renderMap(const Map&, int, int, int);
		~MapRenderer( );
	private:
		MapRenderer(int, int, const layer_t&, const layer_t&, const layer_t&);
		static void prerenderLayer(int, int, SDLImage&, const layer_t&);

		SDLImage map_bottom, map_top;
		layer_t animation;
		int width, height;
};

#endif

