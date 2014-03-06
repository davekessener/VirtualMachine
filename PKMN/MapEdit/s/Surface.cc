#include "Surface.h"

Surface::Surface(Image *i, int x, int y)
	: surface(i), _x(x), _y(y), _w(i->width()), _h(i->height()), isDirty(true)
{
}

Surface::~Surface(void)
{
	delete surface;
}

void Surface::redraw(void)
{
	if(isDirty)
	{
		draw();
		isDirty = false;
	}

	for(Surface *s : surfaces)
	{
		s->redraw();
	}
}

void Surface::forceRedraw(void)
{
	draw();
	isDirty = false;

	for(Surface *s : surfaces)
	{
		s->forceRedraw();
	}
}

bool Surface::hit(int x, int y)
{
	return x >= _x && x < _x + _w && y >= _y && y < _y + _h;
}

Surface *Surface::lock(int x, int y)
{
	x -= _x;
	y -= _y;

	for(Surface *s : surfaces)
	{
		if(s->hit(x, y)) return s->lock(x, y);
	}

	return &*this;
}

void Surface::mouseDown(button_t b, int x, int y)
{
}

void Surface::mouseDrag(button_t b, int x, int y)
{
}

void Surface::mouseUp(button_t b, int x, int y)
{
}

void Surface::registerSurface(Surface *s)
{
	if(s) surfaces.push_back(s);
}

