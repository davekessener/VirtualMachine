#include "SHScrollBar.h"

SHScrollBar::SHScrollBar(Image *i, int x, int y, int w, int r)
	: SScrollBar(i, x, y, w, TILE_SIZE, r, getIconNames())
{
}

SHScrollBar::~SHScrollBar(void)
{
}

void SHScrollBar::mouseDrag(button_t b, int x, int y)
{
	calculatePosition(x - _x, _w);
}

SScrollBar::names_arr SHScrollBar::getIconNames(void)
{
	static SScrollBar::names_arr a;

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

