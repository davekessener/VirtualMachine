#ifndef EDITOR_MAP_H
#define EDITOR_MAP_H

#include <Surface/Scrollable.h>
#include "MapData.h"

namespace editor
{
	class Map : public surface::Scrollable
	{
		public:
			Map(MapData *, Image *, int, int, int, int);
			~Map( );
		protected:
			void draw(Image *, int, int);
			std::pair<int, int> getScrollRanges( );
		private:
			Image *_map, *_grid;
			MapData *_data;
	};
}

#endif

