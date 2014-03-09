#ifndef _TILESET_H
#define _TILESET_H

#include <Surface/Scrollable.h>

class Tileset : public surface::Scrollable
{
	public:
		Tileset(Image *, int, int, int, int);
		~Tileset( );
		void mouseDown(button_t, int, int);
		void setBackgroundColor(color_rgba_t c) { bgColor = c; }
		void setBackgroundPlain(bool f) { bgPlain = f; }
		void setGrid(bool f) { showGrid = f; }
	protected:
		void draw(Image *, int, int);
		std::pair<int, int> getScrollRanges( );
	private:
		Image *tileset, *bg, *grid;
		color_rgba_t bgColor;
		bool bgPlain, showGrid;
		int select;

		void createBg( );
};

#endif

