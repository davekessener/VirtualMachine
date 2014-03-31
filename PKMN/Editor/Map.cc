#include "Map.h"

namespace editor
{
	Map::Map(MapData *m, Image *i, int x, int y, int w, int h, Selector<int> sel)
		: surface::Scrollable(i, x, y, w, h), _grid(NULL), _data(m), _selected(sel), _cur(0)
	{
		assert(_data);

		_layers.reserve(MapData::LAYERS);

		for(int i = 0 ; i < MapData::LAYERS ; ++i)
		{
			_layers.push_back(Layer((*_data)[i]));
		}

		LOG("In total %d layers loaded for map '%s'(%d) [DONE]", _layers.size(), _data->name.c_str(), _data->ID);

		_grid = new Image(W(), H());

		_grid->startBlit();
		_grid->drawGrid(0x000000ff, TILE_SIZE, TILE_SIZE);
		_grid->endBlit();

	}
	
	Map::~Map(void)
	{
		delete _grid;
	}

	void Map::mouseDown(button_t b, int x, int y)
	{
		if(b == ::Button::LEFT)
		{
			mouseDrag(b, x, y);
		}
		else if(b == ::Button::RIGHT)
		{
			x -= X(); y -= Y();
			x /= TILE_SIZE; y /= TILE_SIZE;

			if(x >= 0 && x < _data->width() && y >= 0 && y < _data->height())
			{
				_selected = _layers[_cur][x][y];
			}
		}
		else if(b == ::Button::MIDDLE)
		{
			x -= X(); y -= Y();
			x /= TILE_SIZE; y /= TILE_SIZE;

			if(x >= 0 && x < _data->width() && y >= 0 && y < _data->height())
			{
				std::vector<std::pair<int, int>> cs;
				const int v = _layers[_cur][x][y];

				auto find = [&cs](int x, int y) -> bool
					{
						for(auto i = cs.cbegin() ; i != cs.cend() ; ++i)
						{
							if(i->first == x && i->second == y) return true;
						}

						cs.push_back(std::make_pair(x, y));

						return false;
					};

				std::function<void(int, int)> run = [this, v, &run, &find](int x, int y)
					{
						if(x < 0 || x >= _data->width() || y < 0 || y >= _data->height()) return;
						if(_layers[_cur][x][y] != v) return;

						if(!find(x, y))
						{
							run(x - 1, y);
							run(x + 1, y);
							run(x, y - 1);
							run(x, y + 1);
						}
					};

				run(x, y);

				for(std::pair<int, int> p : cs)
				{
					_layers[_cur][p.first][p.second] = _selected;
				}

				dirty();
			}
		}
	}

	void Map::mouseDrag(button_t b, int x, int y)
	{
		if(b != ::Button::LEFT) return;

		x -= X(); y -= Y();
		x /= TILE_SIZE; y /= TILE_SIZE;

		if(x >= 0 && x < _data->width() && y >= 0 && y < _data->height())
		{
//			(*_cur)[x][y] = _selected;
			_layers[_cur][x][y] = _selected;

			dirty();
		}
	}

	void Map::selectLayer(int i)
	{
		if(i >= 0 && i < _layers.size())
		{
			_cur = i;
		}
		else
		{
			LOG("ERR: Invalid layer %d selected.", i);
		}
	}
	
	void Map::draw(Image *dI, int dx, int dy)
	{
		bool showGrid = Settings::getBool(Settings::MAP_GRID);
		bool showBlur = Settings::getBool(Settings::MAP_BLUR);
		Rect r(0, 0, std::min(W(), _data->width() * TILE_SIZE), std::min(H(), _data->height() * TILE_SIZE));

		dI->startBlit();

		dI->fillRect(0x000000ff, 0, 0, r.w, r.h);
		
		for(int i = 0 ; i < _layers.size() ; ++i)
		{
			Layer l = _layers[i];

			if(showBlur && i && i == _cur) dI->fillRect(0xffffff7f, 0, 0, dI->width(), dI->height());

			assert(l.image());

			Rect r(dx * TILE_SIZE, dy * TILE_SIZE, 
				std::max(0, std::min(W(), l.image()->width() - dx * TILE_SIZE)), 
				std::max(0, std::min(H(), l.image()->height() - dy * TILE_SIZE)));
			dI->blit(l.image(), Point(0, 0), r);

			if(showBlur && i == _cur) break;
		}

		if(showGrid)
		{
			dI->blit(_grid, Point(0, 0), r);
		}
		
		dI->endBlit();
	}
	
	std::pair<int, int> Map::getScrollRanges(void)
	{
		return std::make_pair(std::max(0, (_data->width() * TILE_SIZE - W()) / TILE_SIZE),
								std::max(0, (_data->height() * TILE_SIZE - H()) / TILE_SIZE));
	}
}

