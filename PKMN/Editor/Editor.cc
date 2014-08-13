#include "Editor.h"

namespace editor
{
	class Win : public surface::Window<Editor>
	{
		public:
			Win(Image *i) : surface::Window<Editor>(i, "MapEditor") { }
		protected:
			void registerCommands(std::function<void(const std::string&, std::function<void(std::vector<std::string>)>)> fn)
			{
				surface::Window<Editor>::getContent().registerCommands(fn);
			}
	};
	
	Editor::Editor(Image *i, int x, int y, int w, int h)
		: surface::Surface(i = new SubImage(i, x, y, w, h)), _project(NULL), _map(NULL)
	{
		int dx = 0;

		registerSurface(_tileset = new surface::Scrolling<Tileset>
			(i, 2 * W() / 3, TILE_SIZE, W() - 2 * W() / 3, H() - TILE_SIZE));

		registerSurface(_tsGrid = new surface::ButtonToggle
			(i, W() - TILE_SIZE, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::TS_GRID, s == surface::Button::PRESSED);
				}, Settings::TS_BUTTON_GRID));
		registerSurface(_mapGrid = new surface::ButtonToggle
			(i, TILE_SIZE * dx++, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::MAP_GRID, s == surface::Button::PRESSED);
				}, Settings::TS_BUTTON_GRID));
		registerSurface(_mapBlur = new surface::ButtonToggle
			(i, TILE_SIZE * dx++, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::MAP_BLUR, s == surface::Button::PRESSED);
				}, Settings::MAP_BUTTON_BLUR));

		surface::RadioButtonGroup lsGroup;
		for(int j = 0 ; j < 4 ; ++j)
		{
			registerSurface(layers_[j] = new surface::ButtonRadio
				(i, TILE_SIZE * dx++, 0, [this, j](surface::Button::button_state s)
					{
						if(_map) _map->getContent().selectLayer(j);
					}, Settings::MAP_BUTTON_LAYERSELECT[j]));

			lsGroup.add(layers_[j]);

			if(!j) layers_[j]->changeState(surface::Button::PRESSED);
		}
		lsGroup.join();

		registerSurface(_undo_btn = new surface::Button
			(i, TILE_SIZE * dx++, 0, [this](surface::Button::button_state s)
				{
					if(_map) _map->getContent().undo();
				}, Settings::MAP_BUTTON_UNDO));
		registerSurface(_redo_btn = new surface::Button
			(i, TILE_SIZE * dx++, 0, [this](surface::Button::button_state s)
				{
					if(_map) _map->getContent().redo();
				}, Settings::MAP_BUTTON_REDO));

		_image = i;
	}
	
	Editor::~Editor(void)
	{
		delete _tsGrid;
		delete _mapGrid;
		delete _tileset;
		delete _map;
		delete _project;
	}

	void Editor::registerCommands(std::function<void(const std::string&, std::function<void(std::vector<std::string>)>)> reg)
	{
		typedef std::vector<std::string> params_t;

		reg("open", [this](params_t _p) { _project = new Project(_p.at(1)); });
		reg("map", 
			[this](params_t _p) 
				{
					if(_project)
					{
						openMap(_project->createMap(_p.at(1), 16, 10));
					}
				});
		reg("wid", [this](params_t _p)
			{
				std::istringstream iss(_p.at(1));
				int id;
				iss >> id;

				openMap(static_cast<Project::map_id>(id));
			});
		reg("setlayer", [this](params_t _p) { int l; std::operator>>(std::istringstream(_p.at(1)), l); _map->getContent().selectLayer(l); });
		reg("undo", [this](params_t p_) { if(_map) _map->getContent().undo(); });
		reg("redo", [this](params_t p_) { if(_map) _map->getContent().redo(); });
		reg("save", [this](params_t p_) { if(_project) _project->save(); });
		reg("help", [](params_t p_)
			{
				std::cout << "open filename" << std::endl;
				std::cout << "map mapname - creates testmap, sized 16 x 10" << std::endl;
				std::cout << "wid id - opens map" << std::endl;
				std::cout << "setlayer layerid" << std::endl;
				std::cout << "undo" << std::endl;
				std::cout << "redo" << std::endl;
				std::cout << "save" << std::endl;
				std::cout << "help" << std::endl;
			});
	}
	
	int Editor::run(int argc, char *argv[])
	{
		surface::Application<Win> a(0, 24, 0, -24);
	
		a.run();
	
		return 0;
	}

	void Editor::openMap(Project::map_id id)
	{
		assert(!_map);
		registerSurface(
			_map = 
				new surface::Scrolling<Map>(
					_image, 
					0, TILE_SIZE, 
					2 * W() / 3 - 1, H() - TILE_SIZE,
					[this, id](Image *_i, int _x, int _y, int _w, int _h)->Map*
					{
						return new Map(&*(_project->getMap(id)), _i, _x, _y, _w, _h, 
							Selector<int>(std::bind(&Tileset::get, &_tileset->getContent()), 
								std::bind(&Tileset::set, &_tileset->getContent(), std::placeholders::_1))); 
					}));
	}
}

