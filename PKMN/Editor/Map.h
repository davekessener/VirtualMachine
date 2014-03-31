#ifndef EDITOR_MAP_H
#define EDITOR_MAP_H

#include <Surface/Scrollable.h>
#include <Selector.h>
#include "MapData.h"
#include "Layer.h"

namespace editor
{
	class Map : public surface::Scrollable
	{
		public:
			Map(Image *i, int x, int y, int w, int h) : surface::Scrollable(i, x, y, w, h), _selected([]{return 0;}, [](int){})
				{ assert(!"This constructor should never be invoked!"); }
			Map(MapData *, Image *, int, int, int, int, Selector<int>);
			~Map( );
			void mouseDown(button_t, int, int);
			void mouseDrag(button_t, int, int);
			void selectLayer(int);
		protected:
			void draw(Image *, int, int);
			std::pair<int, int> getScrollRanges( );
		private:
			int _cur;
			Image *_grid;
			MapData *_data;
			std::vector<Layer> _layers;
			Selector<int> _selected;
	};
}

#endif

