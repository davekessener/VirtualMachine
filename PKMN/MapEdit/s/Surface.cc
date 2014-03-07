#include "Surface.h"

namespace surface
{
	Surface::Surface(Image *i, int x, int y)
		: surface(i), _x(x), _y(y), _w(i->width()), _h(i->height()), isDirty(true)
	{
	}
	
	Surface::~Surface(void)
	{
		delete surface;
		surface = NULL;
	}
	
	void Surface::redraw(void)
	{
		if(isDirty)
		{
			draw(surface);
		}
	
		for(Surface *s : surfaces)
		{
			s->redraw();
		}
		
		isDirty = false;
	}
	
	void Surface::forceRedraw(void)
	{
		draw(surface);
	
		for(Surface *s : surfaces)
		{
			s->forceRedraw();
		}

		isDirty = false;
	}
	
	bool Surface::hit(int x, int y)
	{
		return x >= _x && x < _x + _w && y >= _y && y < _y + _h;
	}
	
	Surface *Surface::lock(int x, int y)
	{
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
		if(s)
		{
			s->offset(_x, _y);
			surfaces.push_back(s);
		}
	}
}

