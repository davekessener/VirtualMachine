#include "Map.h"

namespace editor
{
	Map::Map(MapData *m, Image *i, int x, int y, int w, int h)
		: surface::Scrollable(i, x, y, w, h), _map(NULL), _grid(NULL), _data(m)
	{
		assert(_data);

		_layers.reserve(_data->layerCount());

		for(int i = 0 ; i < MapData::LAYERS ; ++i)
		{
			for(map::Layer& l : (*_data)[i])
			{
				_layers.push_back(Layer(l));
			}
		}

		_map = new Image(_data->width() * TILE_SIZE, _data->height() * TILE_SIZE);
		_grid = new Image(W(), H());

		_grid->drawGrid(0x000000ff, TILE_SIZE, TILE_SIZE);
	}
	
	Map::~Map(void)
	{
	}
	
	void Map::draw(Image *dI, int dx, int dy)
	{
		dI->startBlit();
		for(Layer& l : _layers)
		{
			Rect r(dx * TILE_SIZE, dy * TILE_SIZE, 
				std::max(0, std::min(W(), l.image()->width() - dx * TILE_SIZE)), 
				std::max(0, std::min(H(), l.image()->height() - dy * TILE_SIZE)));
			dI->blit(l.image(), Point(0, 0), r);
		}
		dI->endBlit();
	}
	
	std::pair<int, int> Map::getScrollRanges(void)
	{
		return std::make_pair(std::max(0, (_data->width() * TILE_SIZE - W()) / TILE_SIZE),
								std::max(0, (_data->height() * TILE_SIZE - H()) / TILE_SIZE));
	}
}

