#include "Editor.h"

namespace editor
{
	class Win : public surface::Window<Editor>
	{
		public:
			Win(Image *i) : surface::Window<Editor>(i, "MapEditor") { }
	};
	
	Editor::Editor(Image *i, int x, int y, int w, int h)
		: surface::Surface(i = new SubImage(i, x, y, w, h))
	{
		registerSurface(tileset = new surface::Scrolling<Tileset>
			(i, 2 * W() / 3, TILE_SIZE, W() - 2 * W() / 3, H() - TILE_SIZE));
		registerSurface(tsGrid = new surface::ButtonToggle
			(i, W() - TILE_SIZE, 0, [this](surface::Button::button_state s)
				{
					Settings::setBool(Settings::TS_GRID, s == surface::Button::PRESSED);
				}, Settings::TS_BUTTON_GRID));
	}
	
	Editor::~Editor(void)
	{
		delete tsGrid;
		delete tileset;
	}
	
	int Editor::run(int argc, char *argv[])
	{
		surface::Application<Win> a(0, 24, 0, -24);
	
		a.run();
	
		return 0;
	}
}

