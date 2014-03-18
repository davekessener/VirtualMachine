#include "Map.h"

namespace editor
{
	Map::Map(MapData *m, Image *i, int x, int y, int w, int h)
		: surface::Scrollable(i, x, y, w, h), _map(NULL), _grid(NULL), _data(m)
	{
		assert(_data);

		_map = new Image(_data->width() * TILE_SIZE, _data->height() * TILE_SIZE);
		_grid = new Image(W(), H());

		_grid->drawGrid(0x000000ff, TILE_SIZE, TILE_SIZE);
	}
	
	Map::~Map(void)
	{
	}
	
	void Map::draw(Image *dI, int dx, int dy)
	{
	}
	
	std::pair<int, int> Map::getScrollRanges(void)
	{
		return std::make_pair(std::max(0, (_data->width() * TILE_SIZE - W()) / TILE_SIZE),
								std::max(0, (_data->height() * TILE_SIZE - H()) / TILE_SIZE));
	}
}

