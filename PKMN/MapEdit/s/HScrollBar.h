#ifndef _HSCROLLBAR_H
#define _HSCROLLBAR_H

#include "ScrollBar.h"

namespace surface
{
	class HScrollBar : public ScrollBar
	{
		public:
			HScrollBar(Image *, int, int, int, int);
			~HScrollBar( );
			void mouseDrag(button_t, int, int);
		protected:
			names_arr getIconNames( );
		private:
	};
}

#endif

