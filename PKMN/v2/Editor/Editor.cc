#include "Editor.h"
#include <dav/gl.h>
#include <dav/Logger.h>
#include "surface/Menu.h"
#include "surface/FileSelect.h"

#define MXT_TITLE "Editor"
#define MXT_MAXSCREENW 1280
#define MXT_MAXSCREENH 720

namespace editor
{
	using namespace dav;

	int Editor::run(const params_t& args)
	{
		LOG("Start editor [...]");

		Editor e(MXT_MAXSCREENW, MXT_MAXSCREENH);

		sdl::set_init([&e](int w, int h) { e.init(); });
		sdl::set_update([&e](int d) -> bool { e.update(d); return e.running_; });
		sdl::set_input([&e](Controls c, bool b) { e.keyboard(c, b); });
		sdl::set_mouse([&e](MouseButtons b, uint x, uint y, bool d) { e.mouseClick(b, x, y, d); },
					   [&e](uint x, uint y, int dx, int dy) { e.mouseMove(x, y, dx, dy); },
					   [&e](int dx, int dy) { e.mouseWheel(dx, dy); });

		sdl::start(MXT_TITLE, e.width_, e.height_);

		return 0;
	}

	Editor::Editor(uint w, uint h) : width_(w), height_(h)
	{
		using namespace surface;
		Menu *p = new Menu;
		root_.reset(p);
		p->addItem("File", "New",  std::bind(&Editor::quit, this));
		p->addItem("File", "Open", std::bind(&Editor::quit, this));
		p->addItem("File", "Save", std::bind(&Editor::quit, this));
		p->addItem("File", "Quit", std::bind(&Editor::quit, this));
		p->addItem("Edit", "Grid", std::bind(&Editor::quit, this));
		p->addItem("Edit", "Zoom", std::bind(&Editor::quit, this));
		p->addItem("Edit", "Fuck", std::bind(&Editor::quit, this));
		p->addItem("Edit", "You",  std::bind(&Editor::quit, this));
		p->addItem("Edit", "IDEK", std::bind(&Editor::quit, this));
		root_->init(0, 0, w, h);

		dialog_.reset(new FileSelect([this](const std::string& file)
		{
			LOG("Entered file '%s'", file.data());
			quit();
		}));
		dialog_->init(w / 4, h / 4, w / 2, h / 2);
	}

	void Editor::init(void)
	{
		gl::init2d(width_, height_);

		running_ = true;
	}

	void Editor::update(int d)
	{
		bool render = false;

		if(dialog_ && dialog_->hidden()) { dialog_.reset(); render = true; LOG("DIALOG HIDDEN!"); }

		if(root_) render = render || root_->isDirty();
		if(dialog_) render = render || dialog_->isDirty();

		if(root_) root_->update(d);
		if(dialog_) dialog_->update(d);

		if(render)
		{
			gl::start_draw();

			if(root_) root_->render();
			if(dialog_) dialog_->render();

			gl::update();
		}
	}

	void Editor::keyboard(Controls key, bool pressed)
	{
		modifier_.press(key, pressed);

		if(pressed && key == Controls::ESCAPE) quit();

		surface::Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;
		
		if(pressed)
		{
			t->keyDown(key);
		}
		else
		{
			t->keyUp(key);
		}
	}

	void Editor::mouseMove(uint x, uint y, int dx, int dy)
	{
		surface::Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;

		t->mouseMove(x, y);
	}

	void Editor::mouseClick(MouseButtons b, uint x, uint y, bool pressed)
	{
		surface::Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;

		if(pressed)
		{
			t->mouseDown(b, x, y);
		}
		else
		{
			t->mouseUp(b, x, y);
		}
	}

	void Editor::mouseWheel(int dx, int dy)
	{
		surface::Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;

		t->scroll(dy);
	}
}

