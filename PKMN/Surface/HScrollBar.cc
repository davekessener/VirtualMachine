#include "HScrollBar.h"

namespace surface
{
	HScrollBar::HScrollBar(Image *i, int x, int y, int w, int r)
		: ScrollBar(i, x, y, w, TILE_SIZE, r, getIconNames())
	{
	}
	
	HScrollBar::~HScrollBar(void)
	{
	}
	
	void HScrollBar::mouseDrag(button_t b, int x, int y)
	{
		if(b == ::Button::LEFT) calculatePosition(x - X(), W());
	}
	
	ScrollBar::names_arr HScrollBar::getIconNames(void)
	{
		static ScrollBar::names_arr a;
	
		if(a[0].length() == 0)
		{
			a[0] = "scroll_hcursor_1";
			a[1] = "scroll_hcursor_2";
			a[2] = "scroll_fill";
			a[3] = "scroll_left";
			a[4] = "scroll_right";
		}
	
		return a;
	}
}

