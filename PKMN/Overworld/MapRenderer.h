#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <map>
#include <vector>
#include <cstdint>
#include "SDLImage.h"
#include "Map.h"
#include "Pool.h"
#include "Singleton.h"

#ifdef MAPRENDERER_MAIN
#include "Logger.h"
#include "Timer.h"

#define MR_POOL_MAXSIZE 8

class MapRenderer;

template<typename T> class VAge : public PoolValidationAge<T, MR_POOL_MAXSIZE> { };

class _MapRenderer : public Pool<PoolRetainerNew<MapRenderer>, int, VAge>
{
	public:
		_MapRenderer( ) : tileset("./tileset.png") { }
		SDLImage tileset;
};
#else
class _MapRenderer;
#endif

class MapRenderer
{
	typedef std::vector<std::uint32_t> layer_t;

	public:
		static MapRenderer *getRenderer(const Map&);
		static void renderMap(const Map&, int, int, int);
	private:
		MapRenderer(int, int, const layer_t&, const layer_t&, const layer_t&);
		~MapRenderer( );
		MapRenderer( ) : width(-1), height(-1) { }
		static void prerenderLayer(int, int, SDLImage&, const layer_t&);
		bool good( ) { return width > 0 && height > 0; }

		SDLImage map_bottom, map_top;
		layer_t animation;
		int width, height;

		friend class PoolRetainerNew<MapRenderer>;
};

#endif

