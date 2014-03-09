#ifndef _SURFACE_H
#define _SURFACE_H

#include <vector>
#include <functional>
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
			virtual bool capturing( );
			void registerSurface(Surface *);
			Image *getDrawSurface( ) { return surface; }
			void dirty( );
			virtual bool isDirty( );
			inline int X( ) const { return _x; }
			inline int Y( ) const { return _y; }
			inline int W( ) const { return _w; }
			inline int H( ) const { return _h; }
		private:
			const int _x, _y, _w, _h;
			bool _dirty;
			std::vector<Surface *> surfaces;
			button_t pressed;
			Image *surface;
	};
}

#endif

