#include "TilesetWindow.h"

TilesetWindow::TilesetWindow(void)
{
	init();
	if(!(ts = IMG_LoadTexture(getRenderer(), "./tileset.png"))) throw SDLException(IMG_GetError());
	setTitle("Tileset");
}

TilesetWindow::~TilesetWindow(void)
{
	SDL_DestroyTexture(ts);
}

void TilesetWindow::onClick(button_t b, int x, int y)
{
	LOG("Clicked @%d/%d", x, y);
}

void TilesetWindow::draw(void)
{
	SDL_RenderCopy(getRenderer(), ts, NULL, NULL);
}

SDL_Window *TilesetWindow::createWindow(void)
{
	return SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
}

