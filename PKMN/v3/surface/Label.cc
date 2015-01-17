#include "Label.h"

namespace dav { namespace pkmn { namespace screen {

void Label::message(const std::string& m)
{
	if(msg_ != m)
	{
		msg_ = m;
		pieces_.clear();
		dirty(true);
	}
}

void Label::charsize(uint dx)
{
	if(sx_ != dx)
	{
		sy_ = dx + (sy_ - sx_);
		sx_ = dx;
		pieces_.clear();
		dirty(true);
	}
}

void Label::center(bool c)
{
	if(center_ != c)
	{
		center_ = c;
		dirty(true);
	}
}

void Label::i_doPrerender(void)
{
	if(pieces_.empty() && !msg_.empty())
	{
		int mc = width() / sx_;
		const char *c = &msg_.front(), *e = &*msg_.rbegin(); ++e;

		static auto trim = [](const char *s, const char *e) -> std::string
		{
			while(*s == ' ') ++s;
			while(e[-1] == ' ') --e;
			return std::string(s, e);
		};

		while(e - c > mc)
		{
			pieces_.push_back(trim(c, c + mc));
			c += mc;
		}

		pieces_.push_back(std::string(c));
	}
}

void Label::i_doRender(void)
{
	int y = (center_ ? (height() - pieces_.size() * sy_) / 2 : 0) + (sy_ - sx_) / 2;

	for(const std::string& s : pieces_)
	{
		drawString(s, center_ ? (width() - s.size() * sx_) / 2 : 0, y, c_, sx_);
		y += sy_;
	}
}

void Label::i_doResize(void)
{
	pieces_.empty();
}

}}}

