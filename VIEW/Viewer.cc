#include <vector>
#include "Viewer.h"
#include "Randomizer.hpp"
#include "Image.h"
#include "sdl.h"

#define MXT_STEP 16

struct Viewer::Impl
{
	typedef std::vector<std::string> svec;
	typedef svec::const_iterator siter;
	typedef sdl::Controls Controls;
	typedef unsigned int uint;

	Image img_;
	Randomizer<siter> rand_;
	svec files_;
	int dx_ = 0, dy_ = 0;
	int sw_, sh_;
	bool loaded_ = false, scale_ = true;

	render_fn getRender( ) const;
};

Viewer::render_fn Viewer::Impl::getRender(void) const
{
	if(scale_)
	{
		return std::bind(&Image::center, &img_, sw_, sh_);
	}
	else
	{
		return std::bind(&Image::draw, &img_, sw_, sh_, dx_, dy_);
	}
}

Viewer::Viewer(void) : impl_(new Impl)
{
}

Viewer::~Viewer(void)
{
	delete impl_;
}

void Viewer::setScreen(int sw, int sh)
{
	impl_->sw_ = sw;
	impl_->sh_ = sh;
}

void Viewer::add(const std::string& s)
{
	impl_->files_.push_back(s);
}

void Viewer::finalize(void)
{
	impl_->loaded_ = true;
	impl_->rand_.load(impl_->files_.cbegin(), impl_->files_.cend());
	loadImage(impl_->rand_.get());
}

void Viewer::clear(void)
{
	Impl::svec().swap(impl_->files_);
	impl_->rand_.clear();
	impl_->img_.erase();
	impl_->dx_ = impl_->dy_ = 0;
	impl_->loaded_ = false;
}

void Viewer::suspend(bool suspend)
{
	if(!suspend)
	{
		render_ = impl_->getRender();
	}
	else
	{
		render_fn().swap(render_);
	}
}

void Viewer::shift(Direction d, bool s)
{
	if(impl_->scale_) return;

	int w = impl_->img_.width(), h = impl_->img_.height();
	int mx = (w - impl_->sw_);
	int my = (h - impl_->sh_);
	int dx = s ? w / 10 : MXT_STEP, dy = s ? h / 10 : MXT_STEP;
	if(mx < 0) mx = 0;
	if(my < 0) my = 0;

	switch(d)
	{
		case Direction::UP:
				impl_->dy_ += dy;
				if(impl_->dy_ > 0) impl_->dy_ = 0;
			break;
		case Direction::DOWN:
				impl_->dy_ -= dy;
				if(impl_->dy_ < -my) impl_->dy_ = -my;
			break;
		case Direction::LEFT:
				impl_->dx_ += dx;
				if(impl_->dx_ > 0) impl_->dx_ = 0;
			break;
		case Direction::RIGHT:
				impl_->dx_ -= dx;
				if(impl_->dx_ < -mx) impl_->dx_ = -mx;
			break;
	}

	render_ = impl_->getRender();
}

void Viewer::shuffle(void)
{
	impl_->rand_.shuffle(RNGDefault());
}

void Viewer::reset(void)
{
	impl_->rand_.shuffle(RNGStraight());
}

void Viewer::next(void)
{
	impl_->img_.erase();
	impl_->rand_.next();
	loadImage(impl_->rand_.get());
}

void Viewer::back(void)
{
	impl_->img_.erase();
	impl_->rand_.back();
	loadImage(impl_->rand_.get());
}

void Viewer::toggleScale(void)
{
	scale(!impl_->scale_);
}

void Viewer::scale(bool zoom)
{
	if(impl_->scale_ != zoom)
	{
		impl_->scale_ = zoom;
		render_ = impl_->getRender();
	}
}

void Viewer::hide(void)
{
}

void Viewer::loadImage(const std::string& in)
{
	impl_->img_.load(in);
	impl_->img_.create();
	impl_->dx_ = impl_->dy_ = 0;
	
	render_ = impl_->getRender();
}

