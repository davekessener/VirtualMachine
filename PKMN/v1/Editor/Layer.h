#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "../Surface/Image.h"
#include "../Surface/SubImage.h"
#include "MapData.h"
#include "auto_ptr.h"

namespace editor
{
	class Layer
	{
		class _first_indirection;
		class _second_indirection;
		public: class Undo;
		public: class FreezeFrame;

		public:
			Layer(map::Layer);
			Layer(const Layer&);
			~Layer( );
			Layer& operator=(const Layer&);
			void set(int, int, int);
			int get(int, int);
			_first_indirection operator[](int);
			Image *image( );
			auto_ptr<FreezeFrame> freeze( ) const;
			void undo(const Undo&);
			void redo(const Undo&);
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

		public:
			class Undo
			{
				typedef std::vector<std::tuple<int, int, int, int>> vec_t;
				public:
					void add(int x, int y, int u, int r) { data_.push_back(std::tuple<int, int, int, int>(x, y, u, r)); }
					bool empty( ) const { return data_.empty(); }
					vec_t::const_iterator begin( ) const { return data_.begin(); }
					vec_t::const_iterator end( ) const { return data_.end(); }
				private:
					 vec_t data_;
			};

			class FreezeFrame
			{
				typedef std::pair<std::pair<int, int>, std::vector<unsigned int>> backup_t;

				public:
					Undo create(auto_ptr<FreezeFrame>);
				private:
					FreezeFrame( );
					FreezeFrame(const backup_t& b) : backup_(b) { }
					FreezeFrame& operator=(const FreezeFrame&);

					backup_t backup_;

					friend class Layer;
			};
	};
}

#endif

