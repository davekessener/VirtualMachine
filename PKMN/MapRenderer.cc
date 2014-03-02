#define MAPRENDERER_MAIN
#include "MapRenderer.h"
#undef MAPRENDERER_MAIN

std::map<int, std::pair<MapRenderer *, int>> MapRenderer::pool;
SDLImage MapRenderer::tileset;

void MapRenderer::init(void)
{
	tileset.open("./tileset.png");
}

void MapRenderer::quit(void)
{
	for(auto i = pool.begin() ; i != pool.end() ; ++i)
	{
		delete i->second.first;
	}

	pool.clear();
}

MapRenderer *MapRenderer::getRenderer(const Map& map)
{
	if(pool.count(map.ID) > 0)
	{
		if(int p = pool[map.ID].second)
		{
			for(auto i = pool.begin() ; i != pool.end() ; ++i)
			{
				if(i->second.second < p) ++(i->second.second);
			}

			pool[map.ID].second = 0;
		}

		return pool[map.ID].first;
	}
	else
	{
		Timer t;
		MapRenderer *r = new MapRenderer(map.width, map.height, 
			map.getBottomLayer(), map.getAnimationLayer(), map.getTopLayer());
		int e = t.elapsed();

		LOG("Prerendered map #%d '%s' in %dms", map.ID, map.name.c_str(), e);

		for(auto i = pool.begin() ; i != pool.end() ; ++i)
		{
			++(i->second.second);
		}

		pool[map.ID] = std::make_pair(r, 0);

		for(auto i = pool.begin() ; i != pool.end() ; ++i)
		{
			if(i->second.second >= MR_POOL_MAXSIZE)
			{
				pool.erase(i);
				i = pool.begin();
			}
		}

		return r;
	}
}

// # ---------------------------------------------------------------------------

void MapRenderer::renderMap(const Map& map, int ticks)
{
	MapRenderer *render = getRenderer(map);
	SDL_Rect ro, rt;

	int x = 0, y = 0;

	ro.x = x > 0 ? x * 16 : 0;
	ro.y = y > 0 ? y * 16 : 0;

	rt.x = x < 0 ? -x * 16 : 0;
	rt.y = y < 0 ? -y * 16 : 0;

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
	: width(w), height(h), animation(a), map_bottom(w * 16, h * 16), map_top(w * 16, h * 16)
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

				img.blit(tileset, ro, rt);
			}
		}
	}

	img.endBlit();
}

