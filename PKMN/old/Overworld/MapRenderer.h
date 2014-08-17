#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <map>
#include <vector>
#include <cstdint>
#include <Misc/SDLImage.h>
#include "Map.h"
#include "Pool.h"
#include "Singleton.h"
#include "Screen.h"

#ifdef MAPRENDERER_MAIN
#include <Misc/Logger.h>
#include <Misc/Timer.h>

#define MR_POOL_MAXSIZE 8

class MapRenderer;

template<typename T>
class VAge : public PoolCounted<T, MR_POOL_MAXSIZE>, public PoolRemote<T>
{
	public:
		typedef typename PoolSimple<T>::return_type return_type;

		VAge( ) { }
		VAge(return_type v) : PoolSimple<T>(v), PoolCounted<T, MR_POOL_MAXSIZE>(v), PoolRemote<T>(v) { }
		void destroy( ) { PoolSimple<T>::destroy(); }
};

class _MapRenderer : public Pool<MapRenderer, int, VAge>
{
	public:
		_MapRenderer( ) : tileset(Screen::instance().getRenderer(), "./tileset.png") { }
		SDLImage tileset;
};
#endif

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

