#ifndef EDITOR_EDITOR_H
#define EDITOR_EDITOR_H

#include <Surface/Window.h>
#include <Surface/Application.h>
#include <Surface/ButtonToggle.h>
#include "Tileset.h"
#include "Project.h"

namespace editor
{
	class Editor : public surface::Surface
	{
		public:
			Editor(Image *i, int x, int y, int w, int h);
			~Editor( );
			void draw(Image *i) { }
			void registerCommands(std::function<void(const std::string&, std::function<void(std::vector<std::string>)>)>);
			static int run(int, char **);
		private:
			surface::Scrolling<Tileset> *_tileset;
			surface::ButtonToggle *_tsGrid;
	};
}

#endif

