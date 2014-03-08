#include "Image.h"

SDL_Renderer *Image::render = NULL;
TTF_Font *Image::font = NULL;
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
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
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
#define LARP(x1,x2,p,r) ((x1)*((p)/(double)(r))+(x2)*(1.0-(p)/(double)(r)))//(((x1)*(p)+(x2)*((r)-(p)))/(r))
			pix[y * _width + x] = RGBA(
				(int)LARP(LARP(r[0], r[1], y, _height), LARP(r[3], r[2], y, _height), x, _width),
				(int)LARP(LARP(g[0], g[1], y, _height), LARP(g[3], g[2], y, _height), x, _width),
				(int)LARP(LARP(b[0], b[1], y, _height), LARP(b[3], b[2], y, _height), x, _width));
#undef LARP
#undef RGBA
		}
	}

	SDL_UpdateTexture(img, NULL, pix, _width * sizeof(color_rgba_t));

	delete pix;
}

void Image::fillRect(color_rgba_t c, int x, int y, int w, int h)
{
	x = std::max(0, x);
	y = std::max(0, y);
	if(x + w > _width) w = 0;
	if(y + h > _height) h = 0;
	x = std::min(x, _width);
	y = std::min(y, _height);

	if(x == _width || y == _height) return;

	SDL_Rect r = {x, y, w, h};

	SDL_SetRenderDrawColor(render, (c & 0xff000000) >> 24, (c & 0x00ff0000) >> 16, (c & 0x0000ff00) >> 8, 0xff);
	SDL_RenderFillRect(render, &r);
}

void Image::drawLine(color_rgba_t c, int x, int y, int dx, int dy)
{
	x = std::max(0, x);
	y = std::max(0, y);
	if(x + dx > _width) dx = 0;
	if(y + dy > _height) dy = 0;
	x = std::min(x, _width);
	y = std::min(y, _height);

	if(x == _width || y == _height) return;

	SDL_SetRenderDrawColor(render, (c & 0xff000000) >> 24, (c & 0x00ff0000) >> 16, (c & 0x0000ff00) >> 8, 0xff);
	SDL_RenderDrawLine(render, x, y, x + dx - 1, y + dy - 1);
}

void Image::renderText(const std::string& t, int x, int y, color_rgba_t _c)
{
	SDL_Color c = { static_cast<std::uint8_t>((_c & 0xff000000) >> 24), static_cast<std::uint8_t>((_c & 0x00ff0000) >> 16), static_cast<std::uint8_t>((_c & 0x0000ff00) >> 8), 0xff};

	if(x >= _width || y >= _height) throw SDLException("%d|%d in %d, %d", x, y, _width, _height);

	SDL_Surface *s = TTF_RenderText_Blended(font, t.c_str(), c);

	if(!s) throw SDLException(TTF_GetError());

	Image i(s);

	blit(&i, Point(x, y), Rect(0, 0, s->w, s->h));

	SDL_FreeSurface(s);
}

Image::Image(SDL_Surface *s) : _width(s->w), _height(s->h)
{
	if(!(img = SDL_CreateTextureFromSurface(render, s))) throw SDLException();
}

