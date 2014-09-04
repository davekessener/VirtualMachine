#include "Editor.h"
#include <dav/gl.h>
#include <dav/Logger.h>
#include <dav/Timer.h>
#include "File.h"
#include "Tileset.h"
#include "Controller.h"
#include "surface/Menu.h"
#include "surface/FileSelect.h"
#include "surface/Dialog.h"
#include "surface/Manager.h"

#define MXT_TITLE "Editor"
#define MXT_MAXSCREENW (1920-4)
#define MXT_MAXSCREENH (1080-54)

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
		sdl::handle_quit([&e](void) { e.quit(); });

		sdl::start(MXT_TITLE, e.width_, e.height_);

		return 0;
	}

	Editor::Editor(uint w, uint h) : width_(w), height_(h)
	{
		using namespace surface;

		Menu *p = new Menu;
		root_.reset(p);
		p->addItem("File", "New",         std::bind(&Editor::newFile,   this));
		p->addItem("File", "Open ...",    std::bind(&Editor::openFile,  this));
		p->addItem("File", "Save",        std::bind(&Editor::saveFile,  this), std::bind(&Editor::canSave,   this));
		p->addItem("File", "Save As ...", std::bind(&Editor::saveAs,    this), std::bind(&Editor::canSaveAs, this));
		p->addItem("File", "Close",       std::bind(&Editor::closeFile, this), std::bind(&Editor::canClose,  this));
		p->addItem("File", "Quit",        std::bind(&Editor::quit,      this));
		p->addItem("View", "Tileset Grid", []{Tileset::GRID=!Tileset::GRID;}, []{return Controller::isLoaded();});
		p->addItem("View", "Map Grid", []{Controller::GRID=!Controller::GRID;}, []{return Controller::isLoaded();});
		p->addItem("View", "Map Split", []{Controller::SPLIT=!Controller::SPLIT;}, []{return Controller::isLoaded();});
		root_->init(0, 0, w, h);

		file_.reset(new surface::Manager([this](Surface_ptr p, float wr, float hr)
			{
				int w = wr * width_, h = hr * height_;
				dialog_ = p;
				dialog_->init((width_ - w) / 2, (height_ - h) / 2, w, h);
			}));
	}

	void Editor::init(void)
	{
		gl::init2d(width_, height_);

		running_ = true;
	}

	void Editor::update(int d)
	{
		static dav::Timer timer;

		bool render = false;

		if(timer.elapsed() > 250)
		{
			render = true;
			timer.reset();
		}

		if(dialog_ && dialog_->hidden()) { dialog_.reset(); render = true; }

		if(!File::isLoaded() && file_->hasParent()) { root_->removeChild(file_->ID()); render = true; }
		else if(File::isLoaded() && !file_->hasParent())
			{ dynamic_cast<surface::Menu *>(&*root_)->setContent(file_); render = true; }

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
		if(pressed && key == Controls::ESCAPE) quit();

		Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;
		
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
		Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;

		t->mouseMove(x, y);
	}

	void Editor::mouseClick(MouseButtons b, uint x, uint y, bool pressed)
	{
		Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;

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
		Surface_ptr t = static_cast<bool>(dialog_) ? dialog_ : root_;

		t->scroll(dy);
	}

// # ---------------------------------------------------------------------------

	void Editor::newFile(void)
	{
		if(File::isLoaded() && File::hasChanged())
		{
			tryToCloseFile([this](void) { newFile(); });
		}
		else
		{
			File::create();
		}
	}

	void Editor::openFile(void)
	{
		if(File::isLoaded() && File::hasChanged())
		{
			tryToCloseFile([this](void) { openFile(); });
		}
		else
		{
			setFileSelect(Surface_ptr(new surface::FileSelect([this](const std::string& path)
				{
					try
					{
						File::load(path);
					}
					catch(const std::string& e)
					{
						dialog_.reset(new surface::Dialog(e));
						dialog_->init(width_ / 3, height_ / 3, width_ / 3, height_ / 3);
					}
				})));
		}
	}

	void Editor::saveFile(void)
	{
		File::save();
	}

	void Editor::saveAs(void)
	{
		setFileSelect(Surface_ptr(new surface::FileSelect([this](const std::string& path)
			{
				try
				{
					File::save(path);
				}
				catch(const std::string& e)
				{
					setDialog(Surface_ptr(new surface::Dialog(e)));
				}
			})));
	}

	void Editor::closeFile(void)
	{
		if(File::hasChanged())
		{
			tryToCloseFile([this](void) {});
		}
		else
		{
			File::close();
		}
	}

	void Editor::quit(void)
	{
		if(File::isLoaded() && File::hasChanged())
		{
			tryToCloseFile([this](void) { doQuit(); });
		}
		else
		{
			doQuit();
		}
	}

	bool Editor::canSave(void)
	{
		return File::isLoaded() && File::hasChanged() && File::hasName();
	}

	bool Editor::canSaveAs(void)
	{
		return File::isLoaded();
	}

	bool Editor::canClose(void)
	{
		return canSaveAs();
	}

	void Editor::tryToCloseFile(std::function<void(void)> fn)
	{
		setDialog
		(
			Surface_ptr
			(
				new surface::Dialog
				(
					"Save changes?",
					{
						std::make_pair
						(
							"Yes", 
							[this, fn](void)
							{
								if(canSave())
								{
									saveFile();
									File::close();
									fn();
								}
								else
								{
									setFileSelect
									(
										Surface_ptr
										(
											new surface::FileSelect
											(
												[this, fn](const std::string& path)
												{
													try
													{
														File::save(path);
														File::close();
														fn();
													}
													catch(const std::string& e)
													{
														setDialog(Surface_ptr(new surface::Dialog(e)));
													}
												}
											)
										)
									);
								}
							}
						),
						std::make_pair
						(
							"No",
							[fn](void) { File::close(); fn(); }
						)
					}
				)
			)
		);
	}

	void Editor::doQuit(void)
	{
		setDialog(Surface_ptr(new surface::Dialog("Are you sure you want to quit?",
			{
				std::make_pair("Yes", [this](void) { running_ = false; })
			}, "No")));
	}
}

