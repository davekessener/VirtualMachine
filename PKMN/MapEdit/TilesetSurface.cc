#include "TilesetSurface.h"

TilesetSurface::TilesetSurface(int _x, int _y, int _w, int _h)
	: Surface(_x, _y, _w, _h), showG(SHOW_GRID | SIMPLE_BG), grid(getRenderer(), "tsgrid.png"),
	  tileset(getRenderer(), "tileset.png"), bg(getRenderer())
{
	select = 0;

	addQuitButton();

	setBaseColor(255, 0, 255);
}

TilesetSurface::~TilesetSurface(void)
{
}

void TilesetSurface::draw(SDLImage& img, int dx, int dy)
{
	SDL_Rect ro, rt;

	const int w = img.width(), h = img.height();

#define min(i,j) ((i)<(j)?(i):(j))
	ro.x = dx;
	ro.y = dy;
	ro.w = min(w, tileset.width());
	ro.h = min(h, tileset.height());

	rt.x = rt.y = 0;
	rt.w = ro.w;
	rt.h = ro.h;
#undef min

	img.startBlit();

	if(showG & SIMPLE_BG)
	{
		SDL_SetRenderDrawColor(getRenderer(), bgC.r, bgC.g, bgC.b, 255);
		SDL_RenderFillRect(getRenderer(), &rt);
		SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
	}
	else
	{
		img.blit(bg, ro, rt);
	}
	
	img.blit(tileset, ro, rt);
	
	if(showG & SHOW_GRID)
	{
		img.blit(grid, ro, rt);	
	}

	rt.x = ((select % (tileset.width() >> 4)) << 4) + (showG ? 1 : 0) - dx;
	rt.y = ((select / (tileset.width() >> 4)) << 4) + (showG ? 1 : 0) - dy;
	rt.w = rt.h = 15 + (showG ? 0 : 1);

	if(rt.x > -16 && rt.x < ro.w && rt.y > -16 && rt.y < ro.h)
	{
		SDL_SetRenderDrawColor(getRenderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(getRenderer(), &rt);
		SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
	}

	img.endBlit();
}

void TilesetSurface::onClick(button_t b, int _x, int _y)
{
	select = (_y >> 4) * (tileset.width() >> 4) + (_x >> 4);
	LOG("Click @(%d|%d) => selected tile #%d", _x, _y, select);
}

std::pair<int, int> TilesetSurface::getScrollSize(void)
{
#define max(i,j) ((i)>(j)?(i):(j))
	return std::make_pair(max(0, 1024 - width), max(0, 1024 - height));
#undef max
}

void TilesetSurface::setBaseColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	bgC.r = r;
	bgC.g = g;
	bgC.b = b;
	bgC.a = 255;

	bg.create(tileset.width(), tileset.height());

	SDL_Renderer *render = getRenderer();
	SDL_Rect rect;

	rect.x = rect.y = 0;
	rect.w = rect.h = 16;

	SDL_SetRenderTarget(render, static_cast<SDL_Texture *>(bg));
	SDL_SetRenderDrawColor(render, r, g, b, 255);
	SDL_RenderClear(render);
	SDL_SetRenderDrawColor(render, 255 - r, 255 - g, 255 - b, 255);
	
	for(int y = 0 ; y < bg.height() - 32 ; y += 32)
	{
		for(int x = 0 ; x < bg.width() - 32 ; x += 32)
		{
			rect.y = y;
			rect.x = x;

			SDL_RenderFillRect(render, &rect);

			rect.y += 16;
			rect.x += 16;

			SDL_RenderFillRect(render, &rect);
		}
	}

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_SetRenderTarget(render, NULL);
}

