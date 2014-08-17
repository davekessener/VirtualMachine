#define INPUTFIELD_MAIN
#include "InputField.h"
#undef INpUTFIELD_MAIN

InputField::InputField(void) : x_(-1), y_(-1), l_(-1), pos_(0), buf_(NULL), i_(0), idx_(0), len_(0)
{
}

InputField::~InputField(void)
{
	clean();
}

void InputField::start(int x, int y, int l, const char *s)
{
	x_ = x;
	y_ = y;
	l_ = l;

	clean();

	if(s && strlen(s) > 0)
	{
		buf_ = strdup(s);
		i_ = idx_ = strlen(s);
		l_ = i_ + 1;
		while(idx_ - pos_ >= l_) ++pos_;
	}
	else
	{
		resize(BUF_SZ);
	}
}

bool InputField::isRunning(void)
{
	return buf_;
}

void InputField::clean(void)
{
	if(buf_) free(buf_);
	buf_ = NULL;
	i_ = idx_ = len_ = 0;
	pos_ = 0;
}

void InputField::input(int in)
{
	switch(in)
	{
		case display::Keys::LEFT:
			if(idx_ > 0) --idx_;
			break;
		case display::Keys::RIGHT:
			if(idx_ < i_) ++idx_;
			break;
		case display::Keys::UP:
			idx_ = 0;
			pos_ = 0;
			break;
		case display::Keys::DOWN:
			idx_ = i_;
			break;
		case display::Keys::BACKSPACE:
			if(idx_ > 0)
			{
				int i = --idx_;
				while(i++ < i_) buf_[i - 1] = buf_[i];
				buf_[--i_] = '\0';
			}
			break;
		case display::Keys::ESCAPE:
			break;
		case display::Keys::ENTER:
			break;
		default:
			if(isSuitable(in))
			{
				int i = i_;
				while(i-- > idx_) buf_[i + 1] = buf_[i];
				buf_[idx_++] = in;
				if(++i_ == len_ - 1) resize();
			}
			break;
	}

	while(pos_ > idx_) --pos_;
	while(idx_ - pos_ >= l_) ++pos_;
}

std::pair<int, int> InputField::getCurPos(void)
{
	return std::make_pair(x_ + idx_ - pos_, y_);
}

std::pair<int, int> InputField::getPosition(void)
{
	return std::make_pair(x_, y_);
}

std::string InputField::getValue(void)
{
	return std::string(buf_ ? buf_ : "");
}

std::string InputField::getVisible(void)
{
	std::string s = getValue();
	if(pos_ > 0) s = s.substr(pos_);
	if((long)s.length() > l_) s = s.substr(0, l_);

	return s;
}

// # ---------------------------------------------------------------------------

void InputField::resize(int ns)
{
	if(!ns)
	{
		clean();
		return;
	}

	len_ = ns > 0 ? ns : len_ << 1;

	buf_ = static_cast<char *>(realloc(buf_, len_));
	memset(buf_ + i_, 0, len_ - i_);
}

bool InputField::isSuitable(int ch)
{
	if( (ch >= 'a' && ch <= 'z') ||
		(ch >= 'A' && ch <= 'Z') ||
		(ch >= '0' && ch <= '9'))
		return true;

	const char *s = " `~!@#$%^&*()_+-={}|[]\\;:'\"/?.>,<";
	while(*s) if(*s++ == ch) return true;

	return false;
}

