#include "SVScrollBar.h"

SVScrollBar::SVScrollBar(Image *i, int x, int y, int h, int r)
	: SScrollBar(i, x, y, TILE_SIZE, h, r, getIconNames())
{
}

SVScrollBar::~SVScrollBar(void)
{
}

void SVScrollBar::mouseDrag(button_t b, int x, int y)
{
	calculatePosition(y - _y, _h);
}

SScrollBar::names_arr SVScrollBar::getIconNames(void)
{
	static SScrollBar::names_arr a;

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

