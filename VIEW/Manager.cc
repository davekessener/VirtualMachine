#include <set>
#include <stack>
#include <memory>
#include <algorithm>
#include "Manager.h"
#include "inc.h"
#include "GLBuffer.h"
#include "Screen.h"
#include "Viewer.h"
#include "Charset.h"
#include "Input.h"
#include "gl.h"
#include "sdl.h"

#define MXT_WINDOWNAME "DaViewer"
#define MXT_WIDTH 1920
#define MXT_HEIGHT 1080
#define MXT_STEP 16
#define MXT_CHARSET "charset.png"

enum class Action
{
	VIEWER,
	NONE
};

struct Manager::Impl
{
	typedef sdl::Controls Controls;
	typedef unsigned int uint;
	typedef std::shared_ptr<Screen> screen_ptr;

	GLBuffer glBuf_;
	std::stack<screen_ptr> screens_;
	Charset charset_;
	std::set<Controls> modifier_;
	bool running_ = true;

	void do_init(int, int);
	bool do_update(int);
	void do_keyboard(Controls, bool);
	void do_mouse(uint, uint, int, int);

	inline bool pressed(Controls c) { return modifier_.find(c) != modifier_.cend(); }
	screen_ptr getScreen(void);

	void viewInput(Controls c);
};

Manager& Manager::instance(void)
{
	static Manager m;
	return m;
}

Viewer *view(nullptr);

int Manager::run(const std::vector<std::string>& args)
{
	typedef Impl::Controls Controls;
	typedef Impl::uint uint;

	view = new Viewer;
	view->setScreen(MXT_WIDTH, MXT_HEIGHT);
	view->load(args.cbegin() + 1, args.cend());

	sdl::set_init(  [this](int w, int h){ impl_->do_init(w, h); });
	sdl::set_update([this](int d){ return impl_->do_update(d); });
	sdl::set_input( [this](Controls c, bool p){ impl_->do_keyboard(c, p); },
				    [this](uint x, uint y, int dx, int dy){ impl_->do_mouse(x, y, dx, dy); });

	sdl::start(MXT_WINDOWNAME, MXT_WIDTH, MXT_HEIGHT);

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

	view->finalize();
	screens_.push(screen_ptr(view));

	charset_.load(MXT_CHARSET);
	charset_.setCharSize(64);
	
	Input *i = new Input;
	i->setScreen(MXT_WIDTH, MXT_HEIGHT);
	screens_.push(screen_ptr(i));
}

bool Manager::Impl::do_update(int d)
{
	gl::start_draw();

	if(screens_.top()->ready())
	{
		screens_.top()->render();
	}

//	Manager::instance().renderCenteredString("This is a test!", MXT_WIDTH / 2, MXT_HEIGHT / 2);
	
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
		else if(c == Controls::RETURN)
		{
			getScreen()->suspend(getScreen()->ready());
		}
		else if(getScreen()->ready())
		{
			getScreen()->keyPress(c, modifier_);
		}
	}
}

void Manager::Impl::do_mouse(uint x, uint y, int dx, int dy)
{
}

Manager::Impl::screen_ptr Manager::Impl::getScreen(void)
{
	assert(!screens_.empty());

	while(!screens_.top()->alive())
	{
		screens_.pop();
	}
	
	return screens_.top();
}

