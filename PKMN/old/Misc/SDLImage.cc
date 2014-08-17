#define SDLIMAGE_MAIN
#include "SDLImage.h"
#undef SDLIMAGE_MAIN

bool SDLImage::isBlitting = false;

SDLImage::SDLImage(SDL_Renderer *r) : render(r), img(NULL)
{
	_width = _height = -1;
}

SDLImage::SDLImage(SDL_Renderer *r, const std::string& path) : render(r), img(NULL)
{
	open(path);
}

SDLImage::SDLImage(SDL_Renderer *r, int w, int h) : render(r), img(NULL)
{
	create(w, h);
}

SDLImage::~SDLImage(void)
{
	if(img)
	{
		close();
	}
}

void SDLImage::create(int w, int h)
{
	if(img) close();

	_width = w;
	_height = h;

	SDL_Renderer *r = render;
	img = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	if(!img) throw SDLException();
	SDL_SetTextureBlendMode(img, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(r, img);
	SDL_RenderClear(r);
	SDL_SetRenderTarget(r, NULL);
}

void SDLImage::open(const std::string& path)
{
	if(img) close();

	img = IMG_LoadTexture(render, path.c_str());

	if(!img) throw SDLException(IMG_GetError());

	SDL_QueryTexture(img, NULL, NULL, &_width, &_height);
}

void SDLImage::close(void)
{
	if(img)
	{
		SDL_DestroyTexture(img);
		img = NULL;
		_width = _height = -1;
	}
}

void SDLImage::startBlit(void)
{
	if(!isBlitting)
	{
		if(!render) throw SDLException("ERR: Can't blit without renderer.");
		SDL_SetRenderTarget(render, img);
		isBlitting = true;
	}
	else throw SDLException("ALREADY BLITTING!");
}

void SDLImage::endBlit(void)
{
	if(isBlitting)
	{
		if(!render) throw SDLException("ERR: Can't blit without renderer.");
		SDL_SetRenderTarget(render, NULL);
		isBlitting = false;
	}
}

void SDLImage::erase(void)
{
	if(!isBlitting) SDL_SetRenderTarget(render, img);
	SDL_RenderClear(render);
	if(!isBlitting) SDL_SetRenderTarget(render, NULL);
}

void SDLImage::blit(const SDLImage& _i, SDL_Rect _ro, SDL_Rect _rt)
{
	if(!isBlitting) SDL_SetRenderTarget(render, img);
	SDL_RenderCopy(render, _i.img, &_ro, &_rt);
	if(!isBlitting) SDL_SetRenderTarget(render, NULL);
}

SDLImage::operator SDL_Texture *(void) const
{
	return img;
}

