#ifndef _EDITOR_H
#define _EDITOR_H

#include <Surface/Window.h>
#include <Surface/Application.h>
#include "TilesetSurface.h"

class Editor : public surface::Surface
{
	public:
		Editor(Image *i, int x, int y, int w, int h);
		~Editor( );
		void draw(Image *i) { }
		static int run(int, char **);
	private:
		surface::Scrolling<Tileset> *tileset;
		surface::Button *tsGrid;
};

#endif

