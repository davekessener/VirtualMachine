#ifndef _MAP_H
#define _MAP_H

#include <Surface/Scrollable.h>

class Map : public surface::Scrollable
{
	public:
		Map(Image *, int, int, int, int);
		~Map( );
	protected:
		void draw(Image *, int, int);
		std::pair<int, int> getScrollRanges( );
	private:
		Image *map, *grid;
};

#endif

