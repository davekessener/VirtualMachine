#include "Scrollable.h"

namespace surface
{
	Scrollable::Scrollable(Image *i, int x, int y, int w, int h, std::pair<int, int> ranges)
		: Surface(new SubImage(i, x, y, w - TILE_SIZE, h - TILE_SIZE), x, y)
	{
		registerSurface(hscroll = new HScrollBar(i, 0, h - TILE_SIZE, w - TILE_SIZE, ranges.first));
		registerSurface(vscroll = new VScrollBar(i, w - TILE_SIZE, 0, h - TILE_SIZE, ranges.second));
	}
	
	Scrollable::~Scrollable(void)
	{
		delete hscroll;
		delete vscroll;
	}
	
	void Scrollable::draw(void)
	{
		draw(hscroll->getRelativePosition(), vscroll->getRelativePosition());
	}
}

