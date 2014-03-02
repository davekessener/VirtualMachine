#define SDLIMAGE_MAIN
#include "SDLImage.h"
#undef SDLIMAGE_MAIN

bool SDLImage::isBlitting = false;

SDLImage::SDLImage(void) : img(NULL)
{
}

SDLImage::SDLImage(const std::string& path) : img(NULL)
{
	open(path);
}

SDLImage::SDLImage(int w, int h) : img(NULL)
{
	create(w, h);
}

void SDLImage::create(int w, int h)
{
	if(img) close();

	SDL_Renderer *r = Screen::instance().getRenderer();
	img = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(img, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(r, img);
	SDL_RenderClear(r);
	SDL_SetRenderTarget(r, NULL);
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

	img = Screen::instance().loadImage(path);
}

void SDLImage::close(void)
{
	if(img)
	{
		SDL_DestroyTexture(img);
		img = NULL;
	}
}

void SDLImage::startBlit(void)
{
	if(!isBlitting)
	{
		SDL_SetRenderTarget(Screen::instance().getRenderer(), img);
		isBlitting = true;
	}
	else throw SDLException("ALREADY BLITTING!");
}

void SDLImage::endBlit(void)
{
	if(isBlitting)
	{
		SDL_SetRenderTarget(Screen::instance().getRenderer(), NULL);
		isBlitting = false;
	}
}

void SDLImage::blit(const SDLImage& _i, SDL_Rect _ro, SDL_Rect _rt)
{
	SDL_Renderer *r = Screen::instance().getRenderer();

	if(!isBlitting) SDL_SetRenderTarget(r, img);
	SDL_RenderCopy(r, _i.img, &_ro, &_rt);
	if(!isBlitting) SDL_SetRenderTarget(r, NULL);
}

SDLImage::operator SDL_Texture *(void) const
{
	return img;
}

void SDLImage::toScreen(void)
{
	SDL_Rect o, d;

	o.x = o.y = 0;
	SDL_QueryTexture(img, NULL, NULL, &o.w, &o.h);

	d.x = d.y = 0;
	d.w = Screen::SCREEN_WIDTH;
	d.h = Screen::SCREEN_HEIGHT;

	Screen::instance().toScreen(img, o, d);
}

