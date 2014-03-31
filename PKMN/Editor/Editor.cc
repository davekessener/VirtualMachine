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
		registerSurface(_tileset = new surface::Scrolling<Tileset>
			(i, 2 * W() / 3, TILE_SIZE, W() - 2 * W() / 3, H() - TILE_SIZE));
		registerSurface(_tsGrid = new surface::ButtonToggle
			(i, W() - TILE_SIZE, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::TS_GRID, s == surface::Button::PRESSED);
				}, Settings::TS_BUTTON_GRID));
		registerSurface(_mapGrid = new surface::ButtonToggle
			(i, 0, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::MAP_GRID, s == surface::Button::PRESSED);
				}, Settings::TS_BUTTON_GRID));
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

