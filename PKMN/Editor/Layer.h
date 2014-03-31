#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include <Surface/Image.h>
#include <Surface/SubImage.h>
#include "MapData.h"

namespace editor
{
	class Layer
	{
		class _first_indirection;
		class _second_indirection;

		public:
			Layer(map::Layer);
			Layer(const Layer&);
			~Layer( );
			Layer& operator=(const Layer&);
			void set(int, int, int);
			int get(int, int);
			_first_indirection operator[](int);
			Image *image( );
		private:
			std::shared_ptr<bool> _dirty;
			map::Layer _layer;
			std::shared_ptr<std::shared_ptr<Image>> _img;

			void dirty( );

// # ---------------------------------------------------------------------------

		class _second_indirection
		{
			public:
				_second_indirection(Layer *l, int x, int y) : _l(l), _x(x), _y(y) { }
				int operator=(int v) { _l->set(_x, _y, v); return v; }
				operator int( ) { return _l->get(_x, _y); }
			private:
				Layer *_l;
				int _x, _y;
		};
		class _first_indirection
		{
			public:
				_first_indirection(Layer *l, int x) : _l(l), _x(x) { }
				_second_indirection operator[](int i) { return _second_indirection(_l, _x, i); }
			private:
				Layer *_l;
				int _x;
		};
	};
}

#endif

