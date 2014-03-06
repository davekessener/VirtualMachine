#include "Image.h"

SDL_Renderer *Image::render = NULL;
bool Image::isBlitting = false;

Image::Image(void) : img(NULL)
{
	_width = _height = -1;
}

Image::Image(const std::string& path) : img(NULL)
{
	open(path);
}

Image::Image(int w, int h) : img(NULL)
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
			std::min(std::min(_width - t.x, r.w), _i->width() - r.x), 
			std::min(std::min(_height - t.y, r.h), _i->height() - r.y)};
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

void Image::gradientFill(color_rgba_t c1, color_rgba_t c2, color_rgba_t c3, color_rgba_t c4)
{
	color_rgba_t *pix = new color_rgba_t[_width * _height];
	int r[4], g[4], b[4]; std::vector<color_rgba_t> _c{c1, c2, c3, c4};
	for(int i = 0 ; i < 4 ; ++i)
	{
		r[i] = (_c[i] & 0xff000000) >> 24;
		g[i] = (_c[i] & 0x00ff0000) >> 16;
		b[i] = (_c[i] & 0x0000ff00) >>  8;
	}

	for(int y = 0 ; y < _height ; ++y)
	{
		for(int x = 0 ; x < _width ; ++x)
		{
#define RGBA(r,g,b) (((r)<<24)|((g)<<16)|((b)<<8)|0xff)
#define LARP(x1,x2,p,r) (((x1)*(p)+(x2)*((r)-(p)))/(r))
			pix[y * _width + x] = RGBA(
				LARP(LARP(r[0], r[1], y, _height), LARP(r[3], r[2], y, _height), x, _width),
				LARP(LARP(g[0], g[1], y, _height), LARP(g[3], g[2], y, _height), x, _width),
				LARP(LARP(b[0], b[1], y, _height), LARP(b[3], b[2], y, _height), x, _width));
#undef LARP
#undef RGBA
		}
	}

	SDL_UpdateTexture(img, NULL, pix, _width * sizeof(color_rgba_t));

	delete pix;
}

