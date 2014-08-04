#include <set>
#include <memory>
#include <algorithm>
#include "Manager.h"
#include "inc.h"
#include "GLBuffer.h"
#include "gl.h"
#include "sdl.h"
#include "image.h"
#include "Randomizer.hpp"

#define MXT_WINDOWNAME "DaViewer"
#define MXT_WIDTH 1920
#define MXT_HEIGHT 1080
#define MXT_STEP 16
#define MXT_HIDEIMAGE "hidden.bmp.gz"

struct Manager::Impl
{
	typedef std::vector<std::string> svec;
	typedef svec::const_iterator siter;
	typedef sdl::Controls Controls;
	typedef unsigned int uint;

	GLBuffer glBuf_;
	Image img_;
	Randomizer<siter> rand_;
	svec files_;
	std::set<Controls> modifier_;
	bool running_ = true, scale_ = true;
	int dx_ = 0, dy_ = 0;

	void do_init(int, int);
	bool do_update(int);
	void do_keyboard(Controls, bool);
	void do_mouse(uint, uint, int, int);
	void loadImage(const std::string&);
	inline bool pressed(Controls c) { return modifier_.find(c) != modifier_.cend(); }
};

Manager& Manager::instance(void)
{
	static Manager m;
	return m;
}

int Manager::run(const std::vector<std::string>& args)
{
	typedef Impl::svec svec;
	typedef Impl::Controls Controls;
	typedef Impl::uint uint;

	svec(args.cbegin() + 1, args.cend()).swap(impl_->files_);
	impl_->rand_.load(impl_->files_.cbegin(), impl_->files_.cend());

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
	impl_->glBuf_.render(u1, v1, u2, v2, x1, y1, x2, y2);
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
}

bool Manager::Impl::do_update(int d)
{
	gl::start_draw();

	if(!img_.loaded())
	{
		loadImage(rand_.get());
	}

	if(scale_)
	{
		img_.center(MXT_WIDTH, MXT_HEIGHT);
	}
	else
	{
		img_.draw(MXT_WIDTH, MXT_HEIGHT, dx_, dy_);
	}

	gl::update();

	return running_;
}

void Manager::Impl::do_keyboard(sdl::Controls c, bool down)
{
	using sdl::Controls;

	if(down)
	{
		int w = img_.width(), h = img_.height();
		int mx = -(MXT_WIDTH - w) / 2;
		int my = -(MXT_HEIGHT - h) / 2;
		bool s = pressed(Controls::SHIFT);
		if(mx < 0) mx = 0;
		if(my < 0) my = 0;

		switch(c)
		{
			case Controls::SPACE:
			case Controls::X:
				img_.erase();
				rand_.next();
				break;
			case Controls::Z:
				img_.erase();
				rand_.back();
				break;
			case Controls::L:
				scale_ = !scale_;
				break;
			case Controls::R:
				if(s) rand_.shuffle(RNGStraight()); else rand_.shuffle(RNGDefault());
				break;
			case Controls::S:
				dy_ -= s ? h / 10 : MXT_STEP;
				if(dy_ < -my) dy_ = -my;
				break;
			case Controls::W:
				dy_ += s ? h / 10 : MXT_STEP;
				if(dy_ > my) dy_ = my;
				break;
			case Controls::D:
				dx_ -= s ? w / 10 : MXT_STEP;
				if(dx_ < -mx) dx_ = -mx;
				break;
			case Controls::A:
				dx_ += s ? w / 10 : MXT_STEP;
				if(dx_ > mx) dx_ = mx;
				break;
			case Controls::ESCAPE:
				running_ = false;
			case Controls::Q:
				img_.erase();
				loadImage(MXT_HIDEIMAGE);
				break;
			default:
				break;
		}
	}

	if(down)
	{
		modifier_.insert(c);
	}
	else
	{
		auto i = modifier_.find(c);
		if(i != modifier_.end()) modifier_.erase(i);
	}
}

void Manager::Impl::do_mouse(uint x, uint y, int dx, int dy)
{
}

void Manager::Impl::loadImage(const std::string& in)
{
	img_.load(in);
	img_.create();
	dx_ = dy_ = 0;
}

