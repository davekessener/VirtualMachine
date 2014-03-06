#include "SScrollable.h"

SScrollable::SScrollable(Image *i, int x, int y, int w, int h, std::pair<int, int> ranges)
	: Surface(new SubImage(i, x, y, w - TILE_SIZE, h - TILE_SIZE), x, y)
{
	registerSurface(hscroll = new SHScrollBar(i, 0, h - TILE_SIZE, w - TILE_SIZE, ranges.first));
	registerSurface(vscroll = new SVScrollBar(i, w - TILE_SIZE, 0, h - TILE_SIZE, ranges.second));
}

SScrollable::~SScrollable(void)
{
}

void SScrollable::draw(void)
{
	draw(hscroll->getRelativePosition(), vscroll->getRelativePosition());
}

