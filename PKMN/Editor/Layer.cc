#include "Layer.h"

namespace editor
{
	Layer::Layer(map::Layer l) : _layer(l), _dirty(new bool), _img(new std::shared_ptr<Image>)
	{
		*_dirty = true;
	}

	Layer::Layer(const Layer& layer) : _layer(layer._layer), _dirty(layer._dirty), _img(layer._img)
	{
	}

	Layer::~Layer(void)
	{
	}

	Layer& Layer::operator=(const Layer& layer)
	{
		_layer = layer._layer;
		_dirty = layer._dirty;
		_img = layer._img;

		return *this;
	}

	void Layer::set(int x, int y, int v)
	{
		assert(x>=0&&x<_layer.width()&&y>=0&&y<_layer.height()&&v>=0&&v<4096);

		if(_layer[x][y] != v) dirty();

		_layer[x][y] = v;
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
		if(*_dirty || !static_cast<bool>(*_img))
		{
			Image *tileset = Settings::getTileset();
			const int w = _layer.width(), h = _layer.height();
			const int d = TILE_SIZE, s = 1024 / d;

			_img->reset(new Image(w * d, h * d));

			LOG("Redraw of layer @%p", &**_img);

			(*_img)->startBlit();
			(*_img)->clear(0x00000000);
			for(int y = 0 ; y < h ; ++y)
			{
				for(int x = 0 ; x < w ; ++x)
				{
					int v = get(x, y);

					if(!v) continue;

					(*_img)->blit(tileset, Point(x * d, y * d), Rect((v % s) * d, (v / s) * d, d, d));
				}
			}
			(*_img)->endBlit();

			*_dirty = false;
		}

		return &**_img;
	}

	void Layer::dirty(void)
	{
		*_dirty = true;
	}
}

