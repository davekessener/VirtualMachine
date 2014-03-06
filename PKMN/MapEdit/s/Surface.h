#ifndef _SURFACE_H
#define _SURFACE_H

#include <vector>
#include "Geometry.h"
#include "Image.h"

class Surface
{
	public:
		Surface(Image *, int, int);
		virtual ~Surface( );
		void redraw( );
		void forceRedraw( );
		bool hit(int, int);
		Surface *lock(int, int);
		virtual void mouseDown(button_t, int, int);
		virtual void mouseDrag(button_t, int, int);
		virtual void mouseUp(button_t, int, int);
	protected:
		virtual void draw( ) = 0;
		void registerSurface(Surface *);
		inline void dirty( ) { isDirty = true; }
		inline Image *getDrawSurface( ) { return surface; }
		const int _x, _y, _w, _h;
	private:
		bool isDirty;
		std::vector<Surface *> surfaces;
		button_t pressed;
		Image *surface;
};

#endif

