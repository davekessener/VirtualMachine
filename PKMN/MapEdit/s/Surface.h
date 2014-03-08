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
			Surface(Image *);
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
			void dirty( );
			inline int X( ) { return _x; }
			inline int Y( ) { return _y; }
			inline int W( ) { return _w; }
			inline int H( ) { return _h; }
		private:
			const int _x, _y, _w, _h;
			bool isDirty;
			std::vector<Surface *> surfaces;
			button_t pressed;
			Image *surface;
	};
}

#endif

