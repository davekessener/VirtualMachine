#include <set>
#include <memory>
#include <algorithm>
#include "Manager.h"
#include "inc.h"
#include "GLBuffer.h"
#include "Viewer.h"
#include "Charset.h"
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

	GLBuffer glBuf_;
	Viewer view_;
	Charset charset_;
	std::set<Controls> modifier_;
	bool running_ = true;

	void do_init(int, int);
	bool do_update(int);
	void do_keyboard(Controls, bool);
	void do_mouse(uint, uint, int, int);
	inline bool pressed(Controls c) { return modifier_.find(c) != modifier_.cend(); }

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

	impl_->view_.setScreen(MXT_WIDTH, MXT_HEIGHT);

	impl_->view_.load(args.cbegin() + 1, args.cend());

	sdl::set_init(  [this](int w, int h){ impl_->do_init(w, h); });
	sdl::set_update([this](int d){ return impl_->do_update(d); });
	sdl::set_input( [this](Controls c, bool p){ impl_->do_keyboard(c, p); },
				    [this](uint x, uint y, int dx, int dy){ impl_->do_mouse(x, y, dx, dy); });

	sdl::start(MXT_WINDOWNAME, MXT_WIDTH, MXT_HEIGHT);

	return 0;
}

void Manager::createTexture(const BYTE *data, int w, int h)
{
	impl_->glBuf_.set(data, w, h);
}

void Manager::draw(float u1, float v1, float u2, float v2, int x1, int y1, int x2, int y2) const
{
	impl_->glBuf_.bind();
	impl_->glBuf_.render(u1, v1, u2, v2, x1, y1, x2, y2);
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

Manager::Manager(void) : impl_(new Impl)
{
}

Manager::~Manager(void)
{
	delete impl_;
}

void Manager::Impl::do_init(int w, int h)
{
	gl::init2d(w, h);

	view_.finalize();
//	view_.suspend();

	charset_.load(MXT_CHARSET);
	charset_.setCharSize(64);
}

bool Manager::Impl::do_update(int d)
{
	gl::start_draw();

	if(view_.ready())
	{
		view_.render();
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
		else if(c == Controls::RETURN)
		{
			view_.suspend(false);
		}
		else if(view_.ready())
		{
			viewInput(c);
		}
	}
}

void Manager::Impl::do_mouse(uint x, uint y, int dx, int dy)
{
}

void Manager::Impl::viewInput(Controls c)
{
	bool s = pressed(Controls::SHIFT);

	switch(c)
	{
		case Controls::SPACE:
		case Controls::X:
			view_.next();
			break;
		case Controls::Z:
			view_.back();
			break;
		case Controls::L:
			view_.toggleScale();
			break;
		case Controls::R:
			if(s) view_.reset(); else view_.shuffle();
			break;
		case Controls::W:
			view_.shift(Viewer::Direction::UP, s);
			break;
		case Controls::S:
			view_.shift(Viewer::Direction::DOWN, s);
			break;
		case Controls::A:
			view_.shift(Viewer::Direction::LEFT, s);
			break;
		case Controls::D:
			view_.shift(Viewer::Direction::RIGHT, s);
			break;
		case Controls::Q:
			view_.hide();
			break;
		default:
			break;
	}
}

