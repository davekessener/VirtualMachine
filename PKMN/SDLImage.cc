#define SDLIMAGE_MAIN
#include "SDLImage.h"
#undef SDLIMAGE_MAIN

SDLImage::SDLImage(void) : img(NULL)
{
}

SDLImage::SDLImage(const std::string& path) : img(NULL)
{
	open(path);
}

SDLImage::~SDLImage(void)
{
	if(img)
	{
		close();
	}
}

void SDLImage::open(const std::string& path)
{
	if(img) close();

	img = Screen::instance().loadOptimizedImage(path);
}

void SDLImage::close(void)
{
	if(img)
	{
		SDL_FreeSurface(img);
		img = NULL;
	}
}

void SDLImage::blit(const SDLImage& _i, SDL_Rect _ro, SDL_Rect _rt)
{
}

SDLImage::operator SDL_Surface *(void)
{
	return img;
}

void SDLImage::toScreen(void)
{
	SDL_Rect o, d;

	o.x = o.y = 0;
	o.w = img->w;
	o.h = img->h;

	d.x = d.y = 0;
	d.w = Screen::SCREEN_WIDTH;
	d.h = Screen::SCREEN_HEIGHT;

	Screen::instance().toScreen(img, o, d);
}

