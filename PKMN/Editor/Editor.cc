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
		: surface::Surface(i = new SubImage(i, x, y, w, h))
	{
		registerSurface(_tileset = new surface::Scrolling<Tileset>
			(i, 2 * W() / 3, TILE_SIZE, W() - 2 * W() / 3, H() - TILE_SIZE));
		registerSurface(_tsGrid = new surface::ButtonToggle
			(i, W() - TILE_SIZE, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::TS_GRID, s == surface::Button::PRESSED);
				}, Settings::TS_BUTTON_GRID));
	}
	
	Editor::~Editor(void)
	{
		delete _tsGrid;
		delete _tileset;
	}

	void Editor::registerCommands(std::function<void(const std::string&, std::function<void(std::vector<std::string>)>)> reg)
	{
		typedef std::vector<std::string> params_t;

		reg("test", [](params_t _p) { Project p("tstp.nbt"); p.createMap("Pallet Town", 24, 20); p.save(); });
	}
	
	int Editor::run(int argc, char *argv[])
	{
		surface::Application<Win> a(0, 24, 0, -24);
	
		a.run();
	
		return 0;
	}
}

