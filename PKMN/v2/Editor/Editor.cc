#include "Editor.h"
#include <dav/gl.h>

#define MXT_TITLE "Editor"
#define MXT_MAXSCREENW 1280
#define MXT_MAXSCREENH 720

namespace editor
{
	using namespace dav;

	int Editor::run(const params_t& args)
	{
		Editor e(MXT_MAXSCREENW, MXT_MAXSCREENH);

		sdl::set_init([&e](int w, int h) { e.init(); });
		sdl::set_update([&e](int d) -> bool { e.update(); return e.running_; });
		sdl::set_input([&e](Controls c, bool b) { e.keyboard(c, b); });
		sdl::set_mouse([&e](MouseButtons b, uint x, uint y, bool d) { e.mouseClick(b, x, y, d); },
					   [&e](uint x, uint y, int dx, int dy) { e.mouseMove(x, y, dx, dy); },
					   [&e](int dx, int dy) { e.mouseWheel(dx, dy); });

		sdl::start(MXT_TITLE, e.width_, e.height_);

		return 0;
	}

	Editor::Editor(uint w, uint h) : width_(w), height_(h)
	{
	}

	void Editor::init(void)
	{
		gl::init2d(width_, height_);

		running_ = true;
	}

	void Editor::update(void)
	{
		gl::start_draw();

		gl::update();
	}

	void Editor::keyboard(Controls key, bool pressed)
	{
		modifier_.press(key, pressed);

		if(pressed && key == Controls::ESCAPE) quit();
	}

	void Editor::mouseMove(uint x, uint y, int dx, int dy)
	{
	}

	void Editor::mouseClick(MouseButtons b, uint x, uint y, bool pressed)
	{
	}

	void Editor::mouseWheel(int dx, int dy)
	{
	}
}

