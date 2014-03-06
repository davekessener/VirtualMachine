#include "SubImage.h"

SubImage::SubImage(Image *i, int x, int y, int w, int h) : Image(), _i(i)
{
	_x = max(0, min(x, i->width()));
	_y = max(0, min(y, i->height()));
	_width = max(0, min(w, i->width() - _x));
	_height = max(0, min(h, i->height() - _y));
}

SubImage::~SubImage(void)
{
}

void SubImage::blit(const Image *i, Point p, Rect r)
{
	p.x += _x;
	p.y += _y;

	_i->blit(i, p, r);
}

void SubImage::getUnderlying(SDL_Rect *r) const
{
	r->x += _x;
	r->y += _y;
}

SubImage::operator SDL_Texture *(void) const
{
	return static_cast<SDL_Texture *>(*_i);
}

