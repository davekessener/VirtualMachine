#include "StringInput.h"
#include "../Text.h"
#include <dav/Logger.h>
#include <dav/gl.h>

namespace editor { namespace surface {

void StringInput::i_doInit(void)
{
	ml_ = width() / Text::C_W - 1;
	Focus(ID());
	setInput("");
}

void StringInput::i_doUpdate(int d)
{
}

void StringInput::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	dav::gl::draw_rect(p.x, p.y, q.x, q.y, 0x808080);
	dav::gl::draw_rect(p.x + 1, p.y + 1, q.x, q.y, 0xd4d0c8);
	dav::gl::draw_rect(p.x + 1, p.y + 1, q.x - 1, q.y - 1, 0x404040);
	dav::gl::draw_rect(p.x + 2, p.y + 2, q.x - 1, q.y - 1, 0xffffff);

	std::string s(str_.substr(o_));
	if((long)s.length() > ml_) s = s.substr(0, ml_);

	Text::drawText(p.x + Text::C_W / 2, (p.y + q.y) / 2 - Text::C_W / 2, s);
	Text::drawText(p.x + Text::C_W / 2 + (idx_ - o_) * Text::C_W, (p.y + q.y) / 2 - Text::C_W / 2 + 1, "_");
}

void StringInput::i_doMouseUp(MouseButtons b, int x, int y)
{
}

void StringInput::i_doKeyDown(Controls key)
{
	DWORD v = static_cast<DWORD>(key);
	std::string str(str_);
	int idx(idx_), o(o_);
	bool shift(IsKeyPressed(Controls::SHIFT));

	auto insert = [this](char c)
	{
		str_.insert(str_.begin() + idx_, c);
		++idx_;
	};

	if(v >= 'a' && v <= 'z')
	{
		insert(shift ? v + 'A' - 'a' : v);
	}
	else if(v >= '0' && v <= '9')
	{
		const char *tr = ")!@#$%^&*(";
		insert(shift ? tr[v - '0'] : v);
	}
	else switch(key)
	{
		case Controls::UP:
			idx_ = str_.length();
			o_ = (long)str_.length() > ml_ ? str_.length() - ml_ : 0;
			break;
		case Controls::DOWN:
			idx_ = o_ = 0;
			break;
		case Controls::LEFT:
			--idx_;
			break;
		case Controls::RIGHT:
			++idx_;
			break;
		case Controls::BACKSPACE:
			if(idx_ > 0)
			{
				str_ = str_.substr(0, idx_ - 1) + str_.substr(idx_);
				--idx_;
			}
			break;
		case Controls::DELETE:
			if(idx_ < (long)str_.length())
			{
				str_ = str_.substr(0, idx_) + str_.substr(idx_ + 1);
			}
			break;
		case Controls::RETURN:
			if(sig_)
			{
				sig_();
				hide();
			}
			break;
		case Controls::SPACE:  insert(' '); break;
		case Controls::DASH:   insert(shift ? '_' : '-'); break;
		case Controls::PERIOD: insert('.'); break;
		case Controls::SLASH:  insert('/'); break;
		default:
			break;
	}

	while(idx_ <= o_) o_ -= ml_ / 4;
	while(idx_ - o_ >= ml_) o_ += ml_ / 4;
	
	if(o_ >= (long)str_.length()) o_ = str_.length() - 1;
	if(o_ < 0) o_ = 0;
	if(idx_ > (long)str_.length()) idx_ = str_.length();
	if(idx_ < 0) idx_ = 0;

	if(str != str_ || idx != idx_ || o != o_) dirty();
}

}}

