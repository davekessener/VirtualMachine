#ifndef EDITOR_EDITOR_H
#define EDITOR_EDITOR_H

#include "../Surface/Window.h"
#include "../Surface/Application.h"
#include "../Surface/ButtonToggle.h"
#include "../Surface/ButtonRadio.h"
#include "../Selector.h"
#include "Tileset.h"
#include "Map.h"
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
			void openMap(Project::map_id);
			surface::Scrolling<Tileset> *_tileset;
			surface::Scrolling<Map> *_map;
			surface::Button *_undo_btn, *_redo_btn;
			surface::ButtonToggle *_tsGrid, *_mapGrid, *_mapBlur;
			surface::ButtonRadio *layers_[4];
			Project *_project;
			Image *_image;
	};
}

#endif

