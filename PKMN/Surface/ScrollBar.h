#ifndef _SCROLLBAR_H
#define _SCROLLBAR_H

#include <array>
#include <functional>
#include "Surface.h"
#include "Button.h"

namespace surface
{
	class ScrollBar : public Surface
	{
		protected:
		typedef std::array<std::string, 5> names_arr;
	
		public:
			ScrollBar(Image *, int, int, int, int, int, names_arr);
			virtual ~ScrollBar( );
			inline int getRelativePosition( ) { return position; }
			void mouseDown(button_t, int, int);
		protected:
			void draw(Image *);
			virtual names_arr getIconNames( ) = 0;
			void calculatePosition(int, int);
		private:
			void setPosition(int);
			int position, range;
			Image *cursor[2], *fill;
			Button *incr, *decr;
	};
}

#endif

