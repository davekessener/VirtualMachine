#include "MapEditor.h"

class Win : public surface::Window<Editor>
{
	public:
		Win(Image *i) : surface::Window<Editor>(i, "MapEditor") { }
};

Editor::Editor(Image *i, int x, int y, int w, int h) : surface::Surface(i = new SubImage(i, x, y, w, h))
{
	registerSurface(tileset = new surface::Scrolling<Tileset>(i, 2 * W() / 3, TILE_SIZE, W() - 2 * W() / 3, H() - TILE_SIZE));
}

Editor::~Editor(void)
{
	delete tileset;
}

int Editor::run(int argc, char *argv[])
{
	surface::Application<Win> a(0, 24, 0, -24);

	a.run();

	return 0;
}

