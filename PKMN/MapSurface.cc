#include "MapSurface.h"

Map::Map(Image *i, int x, int y, int w, int h) : surface::Scrollable(i, x, y, w, h)
{
}

Map::~Map(void)
{
}

void Map::draw(Image *dI, int dx, int dy)
{
}

std::pair<int, int> Map::getScrollRanges(void)
{
	return std::make_pair(0, 0);
}

