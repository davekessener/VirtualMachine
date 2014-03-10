#ifndef EDITOR_EDITOR_H
#define EDITOR_EDITOR_H

#include <Surface/Window.h>
#include <Surface/Application.h>
#include <Surface/ButtonToggle.h>
#include "Tileset.h"

namespace editor
{
	class Editor : public surface::Surface
	{
		public:
			Editor(Image *i, int x, int y, int w, int h);
			~Editor( );
			void draw(Image *i) { }
			static int run(int, char **);
		private:
			surface::Scrolling<Tileset> *tileset;
			surface::ButtonToggle *tsGrid;
	};
}

#endif

