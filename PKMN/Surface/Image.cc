#include "Image.h"

SDL_Renderer *Image::render = NULL;
std::stack<SDL_Texture *> Image::blitting;

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
	SDL_Texture *target = static_cast<SDL_Texture *>(*this);

	if(!render) throw SDLException("ERR: Can't blit without renderer.");

	LOG("Blitting with '%p' [...]", target);

	if(!blitting.empty() && blitting.top() == target)
	{
		blitting.push(target);
		return;
	}

	blitting.push(target);

	SDL_SetRenderTarget(render, target);
}

void Image::endBlit(void)
{
	if(!render) throw SDLException("ERR: Can't blit without renderer.");

	if(blitting.empty())
	{
		LOG("Isn't blitting!");
		return;
	}

	SDL_Texture *target = blitting.top();

	LOG("Stop blitting with '%p' [DONE]", target);

	blitting.pop();

	if(blitting.empty() || target != blitting.top())
	{
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_SetRenderTarget(render, blitting.empty() ? NULL : blitting.top());
	}
}

void Image::checkBlit(void)
{
	if(blitting.empty() || blitting.top() != static_cast<SDL_Texture *>(*this))
	{
		assert(!"ERR: Needs to be blitting!");
	}
}

void Image::blit(const Image *_i, Point t, Rect r)
{
	checkBlit();

	if(r.x < _i->width() && r.y < _i->height())
	{
		SDL_Rect rt = {t.x, t.y, 
			std::min(std::min(_width - t.x, r.w), _i->width() - r.x), 
			std::min(std::min(_height - t.y, r.h), _i->height() - r.y)};
		SDL_Rect ro = {r.x + _i->X(), r.y + _i->Y(), rt.w, rt.h};

		SDL_RenderCopy(render, static_cast<SDL_Texture *>(*_i), &ro, &rt);
	}
}

Image::operator SDL_Texture *(void) const
{
	return img;
}

void Image::gradientFill(color_rgba_t c1, color_rgba_t c2, color_rgba_t c3, color_rgba_t c4)
{
	checkBlit();

	unsigned int *pix = new unsigned int[_width * _height];
	unsigned int r[4], g[4], b[4];
	std::vector<color_rgba_t> _c{c1, c2, c3, c4};
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

	SDL_UpdateTexture(img, NULL, pix, _width * sizeof(unsigned int));

	delete[] pix;
}

void Image::drawRect(color_rgba_t c, int x, int y, int w, int h)
{
	checkBlit();

	if(x <= -w || y <= -h) return;
	if(x + w > _width) w = _width - x;
	if(y + h > _height) h = _height - y;
	x = std::min(x, _width);
	y = std::min(y, _height);

	if(x == _width || y == _height || w < 0 || h < 0) throw SDLException("Incorrect dimensions: @(%d|%d), %d x %d inside %d x %d", x, y, w, h, _width, _height);

	SDL_Rect r = {x + X(), y + Y(), w, h};

	SDL_SetRenderDrawColor(render, c.r(), c.g(), c.b(), c.a());
	SDL_RenderDrawRect(render, &r);
}

void Image::fillRect(color_rgba_t c, int x, int y, int w, int h)
{
	checkBlit();

	x = std::max(0, x);
	y = std::max(0, y);
	if(x + w > _width) w = _width - x;
	if(y + h > _height) h = _height - y;
	x = std::min(x, _width);
	y = std::min(y, _height);

	if(x == _width || y == _height || w < 0 || h < 0)
	{
		LOG("Incorrect dimensions: @(%d|%d), %d x %d inside %d x %d", x, y, w, h, _width, _height);
		return;
	}

	SDL_Rect r = {x + X(), y + Y(), w, h};

	SDL_SetRenderDrawColor(render, c.r(), c.g(), c.b(), c.a());
	SDL_RenderFillRect(render, &r);
}

void Image::drawLine(color_rgba_t c, int x, int y, int dx, int dy)
{
	checkBlit();

	x = std::max(0, x);
	y = std::max(0, y);
	if(x + dx > _width) dx = _width - x;
	if(y + dy > _height) dy = _height - y;
	x = std::min(x, _width);
	y = std::min(y, _height);

	if(x == _width || y == _height || dx < 0 || dy < 0) throw SDLException("Incorrect dimensions: @(%d|%d), %d x %d inside %d x %d", x, y, dx, dy, _width, _height);

	x += X();
	y += Y();

	SDL_SetRenderDrawColor(render, c.r(), c.g(), c.b(), c.a());
	SDL_RenderDrawLine(render, x, y, x + dx - 1, y + dy - 1);
}

void Image::renderText(const std::string& t, int x, int y, color_rgba_t _c)
{
	checkBlit();

	SDL_Color c = static_cast<SDL_Color>(_c);

	if(x >= _width || y >= _height) throw SDLException("%d|%d in %d, %d", x, y, _width, _height);

	SDL_Surface *s = TTF_RenderText_Blended(FontManager::getFont("tahoma", true, 12), t.c_str(), c);

	if(!s) throw SDLException(TTF_GetError());

	Image i(s);

	blit(&i, Point(x, y), Rect(0, 0, s->w, s->h));

	SDL_FreeSurface(s);
}

void Image::clear(color_rgba_t c, bool bytewise)
{
	if(bytewise)
	{
		unsigned int *pix = new unsigned int[_width * _height];

		for(int i = 0 ; i < _width * _height ; ++i)
		{
			pix[i] = c;
		}

		SDL_Rect r = {X(), Y(), _width, _height};

		SDL_UpdateTexture(img, &r, pix, _width * sizeof(unsigned int));

		delete[] pix;
	}
	else
	{
		checkBlit();

		SDL_SetRenderDrawColor(render, c.r(), c.g(), c.b(), c.a());
		SDL_RenderClear(render);
	}
}

void Image::drawGrid(color_rgba_t c, int dx, int dy)
{
	checkBlit();

	Image cell(dx, dy);
	unsigned int *pix = new unsigned int[dx * dy];

	for(int i = 0 ; i < dx * dy ; ++i)
	{
		pix[i] = (i < dx || i % dx == 0) ? static_cast<unsigned int>(c) : 0;
	}

	SDL_UpdateTexture(static_cast<SDL_Texture *>(cell), NULL, pix, dx * sizeof(unsigned int));

	delete[] pix;

	clear(0, true);

	for(int y = 0 ; y < _height ; y += dy)
	{
		for(int x = 0 ; x < _width ; x += dx)
		{
			blit(&cell, Point(x, y), Rect(0, 0, dx, dy));
		}
	}
}

Image *Image::base(void)
{
	return &*this;
}

Image::Image(SDL_Surface *s) : _width(s->w), _height(s->h)
{
	if(!(img = SDL_CreateTextureFromSurface(render, s))) throw SDLException();
}

