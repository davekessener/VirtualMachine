#include "SubImage.h"

SubImage::SubImage(Image *i) : Image(), _i(i->base()), _x(i->X()), _y(i->Y())
{
	_width = i->width();
	_height = i->height();
}

SubImage::SubImage(Image *i, int x, int y, int w, int h) : Image(), _i(i->base())
{
	_x = std::max(0, std::min(x, i->width())) + i->X();
	_y = std::max(0, std::min(y, i->height())) + i->Y();
	_width = std::max(0, std::min(w, i->width() - (_x - i->X())));
	_height = std::max(0, std::min(h, i->height() - (_y - i->Y())));
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

SubImage::operator SDL_Texture *(void) const
{
	return static_cast<SDL_Texture *>(*_i);
}

Image *SubImage::base(void)
{
	return _i;
}

