#include "Layer.h"

namespace editor
{
	Layer::Layer(map::Layer l) : _layer(l), 
		_img(new Image(l.width() * TILE_SIZE, l.height() * TILE_SIZE))
	{
		Image *tileset = Settings::getTileset();
		const int w = _layer.width(), h = _layer.height();
		const int d = TILE_SIZE, s = 1024 / d;

		_img->startBlit();
		for(int y = 0 ; y < h ; ++y)
		{
			for(int x = 0 ; x < w ; ++x)
			{
				_img->blit(tileset, Point(x * d, y * d), Rect((get(x, y) % s) * d, (get(x, y) / s) * d, d, d));
			}
		}
		_img->endBlit();
	}

	Layer::~Layer(void)
	{
	}

	void Layer::set(int x, int y, int v)
	{
		assert(x>=0&&x<_layer.width()&&y>=0&&y<_layer.height()&&v>=0&&v<4096);
		_layer[x][y] = v;

		const int d = TILE_SIZE, s = 1024 / d;

		_img->startBlit();
		_img->fillRect(0x00000000, x * d, y * d, d, d);
		_img->blit(Settings::getTileset(), Point(x * d, y * d), Rect((v % s) * d, (v / s) * d, d, d));
		_img->endBlit();
	}

	int Layer::get(int x, int y)
	{
		assert(x>=0&&x<_layer.width()&&y>=0&&y<_layer.height());
		return _layer[x][y];
	}

	Layer::_first_indirection Layer::operator[](int i)
	{
		return _first_indirection(this, i);
	}

	Image *Layer::image(void)
	{
		return &*_img;
	}
}

