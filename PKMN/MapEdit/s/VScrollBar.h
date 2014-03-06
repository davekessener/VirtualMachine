#ifndef _VSCROLLBAR_H
#define _VSCROLLBAR_H

#include "ScrollBar.h"

namespace surface
{
	class VScrollBar : public ScrollBar
	{
		public:
			VScrollBar(Image *, int, int, int, int);
			~VScrollBar( );
			void mouseDrag(button_t, int, int);
		protected:
			names_arr getIconNames( );
		private:
	};
}

#endif

