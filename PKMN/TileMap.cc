#define TILEMAP_MAIN
#include "TileMap.h"
#undef TILEMAP_MAIN

Sprite::Sprite(void) : x(0), y(0), dx(0), dy(0), facing(0), sprite("./sprite.png")
{
}

Sprite::~Sprite(void)
{
}

void Sprite::walk(int d)
{
	if(dx || dy) return;

	switch(d)
	{
		case Controls::UP:
			facing = 1;
			--dy;
			break;
		case Controls::DOWN:
			facing = 0;
			++dy;
			break;
		case Controls::LEFT:
			facing = 2;
			--dx;
			break;
		case Controls::RIGHT:
			facing = 2;
			++dx;
			break;
	}
}

void Sprite::update(void)
{
	if(dx < 0) --dx;
	else if(dx > 0) ++dx;
	else if(dy < 0) --dy;
	else if(dy > 0) ++dy;

	while(dx >= 16) { ++x; dx -= 16; }
	while(dy >= 16) { ++y; dy -= 16; }
	while(dx <= -16) { --x; dx += 16; }
	while(dy <= -16) { --y; dy += 16; }
}

void Sprite::render(void) const
{
	SDL_Rect o, d;

	o.w = d.w = 16;
	o.h = d.h = 24;

//	if((dy && (dy >= 8 || -dy >= 8)) || (dx > 0)) { o.w = -o.w; d.w = -d.w; }

	d.x = (Screen::SCREEN_WIDTH - 16) / 2;
	d.y = (Screen::SCREEN_HEIGHT - 16) / 2;

	o.x = 16 * (facing + (dx || dy) ? (3 + (dx >= 8 || -dx >= 8 ? 1 : 0)) : 0);
	o.y = 0;

	Screen::instance().toScreen(static_cast<SDL_Texture *>(sprite), o, d);
}

int Sprite::getX(void) const
{
	return x;
}

int Sprite::getY(void) const
{
	return y;
}

int Sprite::getDX(void) const
{
	return dx;
}

int Sprite::getDY(void) const
{
	return dy;
}

// # ===========================================================================

TileMap::TileMap(int w, int h, std::initializer_list<int> m) : 
	width(w), height(h), tileset("./tileset.png")
{
	int *_m = map = new int[m.size()];

	for(int i : m)
	{
		*_m++ = i;
	}
}

TileMap::~TileMap(void)
{
	delete[] map;
}

void TileMap::render(const Sprite& s)
{
	renderAt(s.getX(), s.getY(), s.getDX(), s.getDY());
	s.render();
}

void TileMap::renderAt(int x, int y, int dx, int dy)
{
	const int CX = Screen::SCREEN_WIDTH / 16, CY = Screen::SCREEN_HEIGHT / 16;
	SDL_Rect o, d;

	while(dx >= 16) dx -= 16;
	while(dy >= 16) dy -= 16;
	while(dx <= -16) dx += 16;
	while(dy <= -16) dy += 16;

	o.w = o.h = d.w = d.h = 16;

	for(int l = 0 ; l < 2 ; ++l)
	{
		for(int _y = -1 ; _y <= CY ; ++_y)
		{
			if(_y + y >= height || _y + y < 0) continue;

			for(int _x = -1 ; _x <= CX ; ++_x)
			{
				if(_x + x >= width || _x + x < 0) continue;

				int v = map[l * width * height + (_y + y) * width + _x + x];

				if(l && !v) continue;

				o.x = (v % 0x40) * 16;
				o.y = (v / 0x40) * 16;

				d.x = _x * 16 - dx;
				d.y = _y * 16 - dy;

				Screen::instance().toScreen(static_cast<SDL_Texture *>(tileset), o, d);
			}
		}
	}
}

