#include "TilesetWindow.h"

TilesetWindow::TilesetWindow(void) : VEdit("Tileset"), select(INVALID)
{
	init();

	ts = IMG_LoadTexture(getRenderer(), "tileset.png");

	if(!ts) throw SDLException(IMG_GetError());
}

TilesetWindow::~TilesetWindow(void)
{
}

void TilesetWindow::onClick(button_t b, int x, int y)
{
	if(x < 0 || y < 0 || x >= 1024 || y >= 1024)
	{
		LOG("Invalid coordinates: %d/%d", x, y);
		return;
	}

	select = (y >> 4) * 0x40 + (x >> 4);
}

void TilesetWindow::redraw(void)
{
	SDL_RenderCopy(getRenderer(), ts, NULL, NULL);
}

SDL_Window *TilesetWindow::createWindow(void)
{
	return SDL_CreateWindow("Tileset", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
}

