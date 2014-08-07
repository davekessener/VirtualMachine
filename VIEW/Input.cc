#include <map>
#include <algorithm>
#include "Input.h"
#include "Manager.h"
#include "gl.h"

struct Input::Impl
{
	DWORD id_;
	int w_, h_, iw_, ih_, s_;
	std::string msg_, prompt_;
	int c_, o_, m_;
	std::map<Controls, char> input_;
	activate_fn activate_;

	void doRender( );
	inline render_fn getRender( ) { return std::bind(&Impl::doRender, this); }
};

void Input::Impl::doRender(void)
{
	int x1 = (w_ - iw_) / 2, y1 = (h_ - ih_) / 2;
	Manager &mngr(Manager::instance());
#define min(a,b) (((a)<(b))?(a):(b))
	std::string out(msg_.c_str() + o_, msg_.c_str() + o_ + min((int)msg_.length() - o_, m_));
#undef min
//	gl::bind_texture(id_);
	gl::draw_rect(0, 0, w_, h_, MXT_BGCOLOR);
	gl::draw_rect(x1, y1, x1 + iw_, y1 + ih_, MXT_WHITE);
	mngr.renderString(prompt_, x1 + s_ / 2 + 1, y1 - s_ - 1, MXT_WHITE);
	mngr.renderString(out, x1 + s_ / 2, y1 + s_ / 4, MXT_BLACK);
	mngr.renderString("_", x1 + s_ / 2 + (c_ - o_) * s_, y1 + s_ / 2 - 1, MXT_BLACK);
}

void Input::setPrompt(const std::string& p)
{
	if((int)p.length() > impl_->m_) 
		impl_->prompt_ = std::string(p.cbegin(), p.cbegin() + impl_->m_);
	else
		impl_->prompt_ = p;
}

void Input::setActivate(activate_fn a)
{
	impl_->activate_ = a;
}

const std::string& Input::getContent(void) const
{
	return impl_->msg_;
}

void Input::setString(const std::string& s, int c, int o)
{
	impl_->msg_ = s;
	if(c >= 0) impl_->c_ = c;
	if(o >= 0) impl_->o_ = o;
}

void Input::moveCursor(int dc)
{
	int c = impl_->c_ + dc;
	int o = impl_->o_;
	if(c < 0) c = 0;
	if(c > (int)impl_->msg_.length()) c = impl_->msg_.length();
	if(c - o >= impl_->m_) o = c - impl_->m_ + 1;
	if(c <= o) o = c - 1;
	if(o < 0) o = 0;
	setString(impl_->msg_, c, o);
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

	if(c == Controls::RETURN)
	{
		if(static_cast<bool>(impl_->activate_))
			impl_->activate_(impl_->msg_);

		setString("", 0, 0);
		return;
	}

	if(char ch = sdl::translate(c, s))
	{
		impl_->msg_.insert(impl_->msg_.begin() + impl_->c_, ch);

		setString(impl_->msg_);
		moveCursor(1);
	}
	else switch(c)
	{
		case Controls::UP:
			moveCursor(impl_->msg_.length());
			break;
		case Controls::DOWN:
			setString(impl_->msg_, 0, 0);
			break;
		case Controls::LEFT:
			moveCursor(-1);
			break;
		case Controls::RIGHT:
			moveCursor(1);
			break;
		case Controls::BACKSPACE:
			if(impl_->c_ > 0 && !impl_->msg_.empty())
			{
				impl_->msg_.erase(impl_->msg_.begin() + impl_->c_ - 1);

				setString(impl_->msg_);

				if(impl_->c_ <= impl_->o_ + 1)
				{
					int c = impl_->c_;
					moveCursor(-impl_->m_ / 4 - 1);
					setString(impl_->msg_, c - 1, impl_->o_);
				}
				else
				{
					moveCursor(-1);
				}
			}
			break;
		case Controls::DELETE:
			if(impl_->c_ < (int)impl_->msg_.length() && !impl_->msg_.empty())
			{
				impl_->msg_.erase(impl_->msg_.begin() + impl_->c_);
				setString(impl_->msg_);
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

