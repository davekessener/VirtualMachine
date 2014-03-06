#ifndef _SVSCROLLBAR_H
#define _SVSCROLLBAR_H

#include "SScrollBar.h"

class SVScrollBar : public SScrollBar
{
	public:
		SVScrollBar(Image *, int, int, int, int);
		~SVScrollBar( );
		void mouseDrag(button_t, int, int);
	protected:
		names_arr getIconNames( );
	private:
};

#endif

