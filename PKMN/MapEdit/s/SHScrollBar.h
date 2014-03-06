#ifndef _SHSCROLLBAR_H
#define _SHSCROLLBAR_H

#include "SScrollBar.h"

class SHScrollBar : public SScrollBar
{
	public:
		SHScrollBar(Image *, int, int, int, int);
		~SHScrollBar( );
		void mouseDrag(button_t, int, int);
	protected:
		names_arr getIconNames( );
	private:
};

#endif

