#include <cstdint>
#include <SDL_image.h>
#include "Image.h"
#include "SDLException.h"

#define MXT_CHAR_S 8

namespace vm { namespace sdl {

// # ===========================================================================

class Image::tex_holder
{
	public:
		~tex_holder( ) { SDL_DestroyTexture(tex_); }
		tex_holder& operator=(SDL_Texture *t) { tex_ = t; return *this; }
		operator SDL_Texture *( ) { return tex_; }
	private:
		SDL_Texture *tex_;
};

// # ---------------------------------------------------------------------------

SDL_Renderer *Image::renderer__ = NULL;
Image::tex_holder Image::charset__;

// # ===========================================================================

Image::Image(void) : tex_(NULL), width_(-1), height_(-1)
{
}

Image::Image(size_t w, size_t h) : tex_(NULL)
{
	create(w, h);
}

Image::Image(const std::string& path) : tex_(NULL)
{
	open(path);
}

Image::~Image(void)
{
	close();
}

void Image::create(size_t w, size_t h)
{
	if(tex_) close();

	SDL_Renderer *r = renderer__;
	
	tex_ = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	width_ = w; height_ = h;
	
	if(!tex_) throw SDLException();
	
	SDL_SetTextureBlendMode(tex_, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(r, tex_);
	SDL_RenderClear(r);
	SDL_SetRenderTarget(r, NULL);
}

void Image::open(const std::string& path)
{
	if(tex_) close();

	tex_ = IMG_LoadTexture(renderer__, path.c_str());

	if(!tex_) throw SDLException(IMG_GetError());

	int w, h;
	SDL_QueryTexture(tex_, NULL, NULL, &w, &h);
	width_ = w; height_ = h;
}

void Image::close(void)
{
	if(tex_)
	{
		SDL_DestroyTexture(tex_);
		tex_ = NULL;
	}
}

size_t Image::width(void) const
{
	return width_;
}

size_t Image::height(void) const
{
	return height_;
}

Image::operator SDL_Texture *(void)
{
	return tex_;
}

void Image::fillRect(color_rgba_t c, int x, int y, int w, int h)
{
	int width = width_, height = height_;
	if(x + w <= 0 || y + h <= 0 || x >= width || y >= height) return;

	if(x < 0) { w += x; x = 0; }
	if(y < 0) { h += y; y = 0; }
	if(x + w > width)  { w = width  - x; }
	if(y + h > height) { h = height - y; }

	SDL_Rect r = {x, y, w, h};

	SDL_SetRenderDrawColor(renderer__, c.r(), c.g(), c.b(), c.a());
	SDL_RenderFillRect(renderer__, &r);
}

void Image::drawChar(color_rgba_t c, int x, int y, char C)
{
	int width = width_, height = height_;
	if(x + MXT_CHAR_S <= 0 || y + MXT_CHAR_S <= 0 || x >= width || y >= height) return;

	int cx = (C & 0xf) * MXT_CHAR_S, cy = (C >> 4) * MXT_CHAR_S;
	int cw = MXT_CHAR_S, ch = MXT_CHAR_S;

	if(x < 0) { cw += x; cx -= x; x = 0; }
	if(y < 0) { ch += y; cy -= y; y = 0; }
	if(x + cw > width)  { cw = width  - x; }
	if(y + ch > height) { ch = height - y; }

	SDL_Rect ro = {cx, cy, cw, ch}, rt = {x, y, cw, ch};

	SDL_SetTextureColorMod(charset__, c.r(), c.g(), c.b());
	SDL_RenderCopy(renderer__, charset__, &ro, &rt);
}

void Image::clear(void)
{
	SDL_SetRenderDrawColor(renderer__, bg_.r(), bg_.g(), bg_.b(), bg_.a());
	SDL_RenderClear(renderer__);
}

void Image::startBlit(void)
{
	SDL_SetRenderTarget(renderer__, tex_);
}

void Image::stopBlit(void)
{
	SDL_SetRenderTarget(renderer__, NULL);
}

void Image::render(void)
{
	SDL_RenderCopy(renderer__, tex_, NULL, NULL);
}

// # ---------------------------------------------------------------------------

void Image::init(SDL_Renderer *r)
{
	renderer__ = r;

	if(!renderer__) throw SDLException();

	charset__ = IMG_LoadTexture(renderer__, "charset.png");

	if(!charset__) throw SDLException(IMG_GetError());
}


}}

