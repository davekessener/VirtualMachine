#include <iostream>
#include <fstream>
#include <set>
#include <stack>
#include <algorithm>
#include <unistd.h>
#include "Manager.h"
#include "inc.h"
#include "GLBuffer.h"
#include "Screen.h"
#include "Dialog.h"
#include "Viewer.h"
#include "Charset.h"
#include "Input.h"
#include "Password.h"
#include "FileInput.h"
#include "Header.h"
#include <dav/gl.h>
#include <dav/sdl.h>

#define MXT_WINDOWNAME "DaViewer"
#define MXT_WIDTH 1920
#define MXT_HEIGHT 1080
#define MXT_STEP 16
#define MXT_CHARSET "./charset.png"

struct Manager::Impl
{
	typedef sdl::Controls Controls;
	typedef unsigned int uint;

	GLBuffer glBuf_;
	std::stack<screen_ptr> screens_;
	Charset charset_;
	std::set<Controls> modifier_;
	bool running_ = true;
	int w_, h_;

	void do_init(int, int);
	bool do_update(int);
	void do_keyboard(Controls, bool);
	void do_mouse(uint, uint, int, int);

	inline bool pressed(Controls c) { return modifier_.find(c) != modifier_.cend(); }
	Screen& getScreen(void);

	void viewInput(Controls c);
};

Manager& Manager::instance(void)
{
	static Manager m;
	return m;
}

int Manager::run(const std::vector<std::string>& args)
{
	typedef Impl::Controls Controls;
	typedef Impl::uint uint;

	sdl::set_init(  [this](int w, int h){ impl_->do_init(w, h); });
	sdl::set_update([this](int d){ return impl_->do_update(d); });
	sdl::set_input( [this](Controls c, bool p){ impl_->do_keyboard(c, p); },
				    [this](uint x, uint y, int dx, int dy){ impl_->do_mouse(x, y, dx, dy); });

	sdl::start(MXT_WINDOWNAME, MXT_WIDTH, MXT_HEIGHT, true);

	return 0;
}

// # ===========================================================================

void Manager::createTexture(const BYTE *data, int w, int h)
{
	impl_->glBuf_.set(data, w, h);
}

void Manager::draw(float u1, float v1, float u2, float v2, int x1, int y1, int x2, int y2) const
{
	impl_->glBuf_.bind();
	impl_->glBuf_.render(u1, v1, u2, v2, x1, y1, x2, y2);
}

void Manager::setCharSize(int s)
{
	impl_->charset_.setCharSize(s);
}

int Manager::getStringWidth(const std::string& s) const
{
	return impl_->charset_.getStringWidth(s);
}

void Manager::renderString(const std::string& s, int x, int y, int c) const
{
	impl_->charset_.renderStringAt(s, x, y, c);
}

void Manager::renderCenteredString(const std::string& s, int x, int y, int c) const
{
	impl_->charset_.renderStringAt(s, 
			x - impl_->charset_.getStringWidth(s) / 2, 
			y - impl_->charset_.getCharHeight() / 2, c);
}

void Manager::pushScreen(screen_ptr p)
{
	p->setScreen(impl_->w_, impl_->h_);
	impl_->screens_.push(p);
}

// # ===========================================================================

Manager::Manager(void) : impl_(new Impl)
{
}

Manager::~Manager(void)
{
	delete impl_;
}

// # ===========================================================================

void Manager::Impl::do_init(int w, int h)
{
	gl::init2d(w, h);

	w_ = w;
	h_ = h;

	charset_.load(MXT_CHARSET);
	charset_.setCharSize(h / 16);
	
	Input *i = new FileInput;
	i->setScreen(w_, h_);
	i->setActivate([](const std::string& fn)
		{
			std::ifstream in(fn);
			if(!in.is_open())
			{
				Screen *s = new Dialog("ERR: File\n'" + fn + "'\ncannot be opened.");
				Manager::instance().pushScreen(Manager::screen_ptr(s));
			}
			else
			{
				Password *p = new Password;
				p->setActivate(Header(in));
				in.close();
				Manager::instance().pushScreen(Manager::screen_ptr(p));
			}

			std::string p(fn.substr(0, fn.find_last_of('/') + 1));
			if(!p.empty()) chdir(p.c_str());
		});
	screens_.push(screen_ptr(i));
}

bool Manager::Impl::do_update(int d)
{
	gl::start_draw();

	Screen &s(getScreen());
	if(s.ready())
	{
		s.render();
	}

	gl::update();

	return running_;
}

void Manager::Impl::do_keyboard(sdl::Controls c, bool down)
{
	using sdl::Controls;

	if(down)
	{
		modifier_.insert(c);
	}
	else
	{
		auto i = modifier_.find(c);
		if(i != modifier_.end()) modifier_.erase(i);
	}

	if(down)
	{
		if(c == Controls::ESCAPE)
		{
			running_ = false;
		}
		else if(getScreen().ready())
		{
			getScreen().keyPress(c, modifier_);
		}
	}
}

void Manager::Impl::do_mouse(uint x, uint y, int dx, int dy)
{
}

Screen& Manager::Impl::getScreen(void)
{
	assert(!screens_.empty());

	while(!screens_.top()->alive())
	{
		screens_.pop();
	}
	
	return *screens_.top();
}

