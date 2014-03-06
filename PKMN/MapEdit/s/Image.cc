#include "Image.h"

bool Image::isBlitting = false;

Image::Image(SDL_Renderer *r) : render(r), img(NULL)
{
	_width = _height = -1;
}

Image::Image(SDL_Renderer *r, const std::string& path) : render(r), img(NULL)
{
	open(path);
}

Image::Image(SDL_Renderer *r, int w, int h) : render(r), img(NULL)
{
	create(w, h);
}

Image::~Image(void)
{
	if(img)
	{
		close();
	}
}

void Image::create(int w, int h)
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

void Image::open(const std::string& path)
{
	if(img) close();

	img = IMG_LoadTexture(render, path.c_str());

	if(!img) throw SDLException(IMG_GetError());

	SDL_QueryTexture(img, NULL, NULL, &_width, &_height);
}

void Image::close(void)
{
	if(img)
	{
		SDL_DestroyTexture(img);
		img = NULL;
		_width = _height = -1;
	}
}

void Image::startBlit(void)
{
	if(!isBlitting)
	{
		if(!render) throw SDLException("ERR: Can't blit without renderer.");
		SDL_SetRenderTarget(render, static_cast<SDL_Texture *>(*this));
		isBlitting = true;
	}
	else throw SDLException("ALREADY BLITTING!");
}

void Image::endBlit(void)
{
	if(isBlitting)
	{
		if(!render) throw SDLException("ERR: Can't blit without renderer.");
		SDL_SetRenderTarget(render, NULL);
		isBlitting = false;
	}
}

void Image::getUnderlying(SDL_Rect *r) const
{
}

void Image::blit(const Image *_i, Point t, Rect r)
{
	if(!isBlitting) SDL_SetRenderTarget(render, img);

	if(r.x < _i->width() && r.y < _i->height())
	{
		SDL_Rect rt = {t.x, t.y, 
			min(min(_width - t.x, r.w), _i->width() - r.x), 
			min(min(_height - t.y, r.h), _i->height() - r.y)};
		SDL_Rect ro = {r.x, r.y, rt.w, rt.h};

		_i->getUnderlying(&ro);

		SDL_RenderCopy(render, static_cast<SDL_Texture *>(*_i), &ro, &rt);
	}

	if(!isBlitting) SDL_SetRenderTarget(render, NULL);
}

Image::operator SDL_Texture *(void) const
{
	return img;
}

