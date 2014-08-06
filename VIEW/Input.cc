#include <map>
#include <algorithm>
#include "Input.h"
#include "Manager.h"
#include "gl.h"

struct Input::Impl
{
	DWORD id_;
	int w_, h_, iw_, ih_, s_;
	std::string msg_;
	int c_, o_, m_;
	std::map<Controls, char> input_;

	void doRender( );
	inline render_fn getRender( ) { return std::bind(&Impl::doRender, this); }
};

void Input::Impl::doRender(void)
{
	int x1 = (w_ - iw_) / 2, y1 = (h_ - ih_) / 2;
#define min(a,b) (((a)<(b))?(a):(b))
	std::string out(msg_.c_str() + o_, msg_.c_str() + o_ + min((int)msg_.length() - o_, m_));
#undef min
//	gl::bind_texture(id_);
	gl::draw_rect(0, 0, w_, h_, 0x6699ff);
	gl::draw_rect(x1, y1, x1 + iw_, y1 + ih_, 0xffffff);
	Manager::instance().renderString(out, x1 + s_ / 2, y1 + s_ / 4, 0x000000);
	Manager::instance().renderString("_", x1 + s_ / 2 + (c_ - o_) * s_, y1 + s_ / 2, 0x000000);
}

void Input::setString(const std::string& s, int c, int o)
{
	impl_->msg_ = s;
	if(c >= 0) impl_->c_ = c;
	if(o >= 0) impl_->o_ = o;
	setRender(impl_->getRender());
}

void Input::i_setScreen(int w, int h)
{
	impl_->w_ = w;
	impl_->h_ = h;
	impl_->s_ = h / 16;
	impl_->iw_ = w * 3 / 4;
	impl_->ih_ = impl_->s_ * 3 / 2;
	impl_->c_ = 0;
	impl_->o_ = 0;
	impl_->m_ = impl_->iw_ / impl_->s_;

	Manager::instance().setCharSize(impl_->s_);

	setRender(impl_->getRender());
}

void Input::i_suspend(bool s)
{
}

void Input::i_keyPress(Controls c, const modifier_t& m)
{
#define pressed(c) (std::find(m.cbegin(),m.cend(),c)!=m.cend())
	bool s = pressed(Controls::SHIFT);
#undef pressed

	if(char ch = sdl::translate(c, s))
	{
		std::string s(impl_->msg_.cbegin(), impl_->msg_.cbegin() + impl_->c_);
		s.push_back(ch);
		s.insert(s.end(), impl_->msg_.cbegin() + impl_->c_, impl_->msg_.cend());
#define max(a,b) (((a)>(b))?(a):(b))
		setString(s, impl_->c_ + 1, impl_->o_ + max(0, impl_->c_ + 2 - impl_->o_ - impl_->m_));
#undef max
	}
	else switch(c)
	{
		case Controls::UP:
			setString(impl_->msg_, impl_->msg_.length(), impl_->msg_.length() - impl_->m_ + 1);
			break;
		case Controls::DOWN:
			setString(impl_->msg_, 0, 0);
			break;
		case Controls::LEFT:
#define max(a,b) (((a)>(b))?(a):(b))
			setString(impl_->msg_, impl_->c_ - 1, impl_->o_ - max(0, impl_->o_ - impl_->c_ + 2));
#undef max
			break;
		case Controls::RIGHT:
			if(impl_->c_ < (int)impl_->msg_.length())
			{
#define max(a,b) (((a)>(b))?(a):(b))
				setString(impl_->msg_, impl_->c_ + 1, impl_->o_ + max(0, impl_->c_ + 2 - impl_->o_ - impl_->m_));
#undef max
			}
			break;
		case Controls::BACKSPACE:
			if(impl_->c_ > 0 && !impl_->msg_.empty())
			{
				std::string s(impl_->msg_.c_str(), impl_->msg_.c_str() + impl_->c_ - 1);
				s.insert(s.end(), impl_->msg_.cbegin() + impl_->c_, impl_->msg_.cend());
#define max(a,b) (((a)>(b))?(a):(b))
				setString(s, impl_->c_ - 1, impl_->o_ - max(0, impl_->o_ - impl_->c_ + 2));
#undef max
			}
			break;
		case Controls::DELETE:
			if(impl_->c_ < (int)impl_->msg_.length() && !impl_->msg_.empty())
			{
				std::string s(impl_->msg_.cbegin(), impl_->msg_.cbegin() + impl_->c_);
				s.insert(s.end(), impl_->msg_.cbegin() + impl_->c_ + 1, impl_->msg_.cend());
				setString(s);
			}
			break;
		default:
			break;
	}
}

Input::Input(void) : impl_(new Impl)
{
}

Input::~Input(void) noexcept
{
	delete impl_;
}

