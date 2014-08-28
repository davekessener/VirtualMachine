#include "MapRenderer.h"
#include "../Misc/Logger.h"
#include "../Misc/Timer.h"

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

typedef Singleton<_MapRenderer> Renderers;

MapRenderer *MapRenderer::getRenderer(const Map& map)
{
	MapRenderer *r = Renderers::instance().getFromPool(map.ID);

	if(!r)
	{
		Timer t;
		r = new MapRenderer(map.width, map.height, 
			map.getBottomLayer(), map.getAnimationLayer(), map.getTopLayer());
		int e = t.elapsed();

		Renderers::instance().addToPool(map.ID, r);

		LOG("Prerendered map #%d '%s' in %dms", map.ID, map.name.c_str(), e);
	}

	return r;
}

// # ---------------------------------------------------------------------------

void MapRenderer::renderMap(const Map& map, int ticks, int x, int y)
{
	MapRenderer *render = getRenderer(map);
	SDL_Rect ro, rt;

	x -= (Screen::SCREEN_WIDTH / 2 - 8) & ~0x0f;
	y -= (Screen::SCREEN_HEIGHT / 2 - 8) & ~0x0f;

	ro.x = x > 0 ? x : 0;
	ro.y = y > 0 ? y : 0;

	rt.x = x < 0 ? -x : 0;
	rt.y = y < 0 ? -y : 0;

	ro.w = render->width * 16 - ro.x;
	ro.h = render->height * 16 - ro.y;

	rt.w = Screen::SCREEN_WIDTH - rt.x;
	rt.h = Screen::SCREEN_HEIGHT - rt.y;

#define min(i,j) ((i)<(j)?(i):(j))
	ro.w = rt.w = min(ro.w, rt.w);
	ro.h = rt.h = min(ro.h, rt.h);
#undef min

	if(ro.w < 0 || ro.h < 0) return;

	Screen::instance().toScreen(static_cast<SDL_Texture *>(render->map_bottom), ro, rt);
	Screen::instance().toScreen(static_cast<SDL_Texture *>(render->map_top), ro, rt);
}

// # ===========================================================================

MapRenderer::MapRenderer(int w, int h, const layer_t& b, const layer_t& a, const layer_t& t) 
	: width(w), height(h), animation(a), map_bottom(Screen::instance().getRenderer(), w * 16, h * 16), map_top(Screen::instance().getRenderer(), w * 16, h * 16)
{
	prerenderLayer(w, h, map_bottom, b);
	prerenderLayer(w, h, map_top, t);
}

MapRenderer::~MapRenderer(void)
{
}

void MapRenderer::prerenderLayer(int w, int h, SDLImage& img, const layer_t& layer)
{
	int v, s = layer.size();
	int lc = (s + w * h - 1) / (w * h);
	SDL_Rect ro, rt;

	SDLImage &ts = Renderers::instance().tileset;

	ro.w = ro.h = rt.w = rt.h = 16;
	ro.x = ro.y = rt.x = rt.y = 0;

	img.startBlit();

	for(int l = 0 ; l < lc ; ++l)
	{
		for(int y = 0 ; y < h ; ++y)
		{
			for(int x = 0 ; x < w ; ++x)
			{
				if(l * w * h + y * w + x >= s) return;

				v = layer[l * w * h + y * w + x];

				if(!v) continue;

				rt.x = 16 * x;
				rt.y = 16 * y;

				ro.x = 16 * (v % 0x40);
				ro.y = 16 * (v / 0x40);

				img.blit(ts, ro, rt);
			}
		}
	}

	img.endBlit();
}

