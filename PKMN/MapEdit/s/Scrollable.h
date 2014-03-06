#ifndef _SCROLLABLE_H
#define _SCROLLABLE_H

#include "Surface.h"
#include "ScrollBar.h"
#include "VScrollBar.h"
#include "HScrollBar.h"

namespace surface
{
	class Scrollable : public Surface
	{
		public:
			Scrollable(Image *, int, int, int, int, std::pair<int, int>);
			~Scrollable( );
		protected:
			void draw( );
			virtual void draw(int, int) = 0;
			virtual std::pair<int, int> getScrollRanges( ) = 0;
		private:
			ScrollBar *vscroll, *hscroll;
	};
}

#endif

