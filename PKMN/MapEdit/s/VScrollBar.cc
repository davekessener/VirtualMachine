#include "VScrollBar.h"

namespace surface
{
	VScrollBar::VScrollBar(Image *i, int x, int y, int h, int r)
		: ScrollBar(i, x, y, TILE_SIZE, h, r, getIconNames())
	{
	}
	
	VScrollBar::~VScrollBar(void)
	{
	}
	
	void VScrollBar::mouseDrag(button_t b, int x, int y)
	{
		if(b == ::Button::LEFT) calculatePosition(y - Y(), H());
	}
	
	ScrollBar::names_arr VScrollBar::getIconNames(void)
	{
		static ScrollBar::names_arr a;
	
		if(a[0].length() == 0)
		{
			a[0] = "scroll_vcursor_1";
			a[1] = "scroll_vcursor_2";
			a[2] = "scroll_fill";
			a[3] = "scroll_up";
			a[4] = "scroll_down";
		}
	
		return a;
	}
}

