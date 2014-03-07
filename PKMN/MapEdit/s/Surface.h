#ifndef _SURFACE_H
#define _SURFACE_H

#include <vector>
#include "Geometry.h"
#include "Image.h"

namespace surface
{
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
			virtual void draw(Image *) = 0;
			void registerSurface(Surface *);
			inline void dirty( ) { isDirty = true; }
			inline void offset(int dx, int dy) { _x += dx; _y += dy; }
			inline int X( ) { return _x; }
			inline int Y( ) { return _y; }
			inline int W( ) { return _w; }
			inline int H( ) { return _h; }
		private:
			int _x, _y, _w, _h;
			bool isDirty;
			std::vector<Surface *> surfaces;
			button_t pressed;
			Image *surface;
	};
}

#endif

