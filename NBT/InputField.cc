#define INPUTFIELD_MAIN
#include "InputField.h"
#undef INpUTFIELD_MAIN

InputField::InputField(void) : x(-1), y(-1), l(-1), pos(0), _buf(NULL), _i(0), _idx(0), _l(0)
{
}

InputField::~InputField(void)
{
	clean();
}

void InputField::start(int _x, int _y, int __l, const char *s)
{
	x = _x;
	y = _y;
	l = __l;

	clean();

	if(s && strlen(s) > 0)
	{
		_buf = strdup(s);
		_i = _idx = strlen(s);
		_l = _i + 1;
		while(_idx - pos >= l) ++pos;
	}
	else
	{
		resize(BUF_SZ);
	}
}

bool InputField::isRunning(void)
{
	return _buf;
}

void InputField::clean(void)
{
	if(_buf) free(_buf);
	_buf = NULL;
	_i = _idx = _l = 0;
	pos = 0;
}

void InputField::input(int in)
{
	switch(in)
	{
		case display::Keys::LEFT:
			if(_idx > 0) --_idx;
			break;
		case display::Keys::RIGHT:
			if(_idx < _i) ++_idx;
			break;
		case display::Keys::UP:
			_idx = 0;
			pos = 0;
			break;
		case display::Keys::DOWN:
			_idx = _i;
			break;
		case display::Keys::BACKSPACE:
			if(_idx > 0)
			{
				int i = --_idx;
				while(i++ < _i) _buf[i - 1] = _buf[i];
				_buf[--_i] = '\0';
			}
			break;
		case display::Keys::ESCAPE:
			break;
		case display::Keys::ENTER:
			break;
		default:
			if(isSuitable(in))
			{
				int i = _i;
				while(i-- > _idx) _buf[i + 1] = _buf[i];
				_buf[_idx++] = in;
				if(++_i == _l - 1) resize();
			}
			break;
	}

	while(pos > _idx) --pos;
	while(_idx - pos >= l) ++pos;
}

std::pair<int, int> InputField::getCurPos(void)
{
	return std::make_pair(x + _idx - pos, y);
}

std::pair<int, int> InputField::getPosition(void)
{
	return std::make_pair(x, y);
}

std::string InputField::getValue(void)
{
	return std::string(_buf ? _buf : "");
}

std::string InputField::getVisible(void)
{
	std::string s = getValue();
	if(pos > 0) s = s.substr(pos);
	if(s.length() > l) s = s.substr(0, l);

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

	_l = ns > 0 ? ns : _l << 1;

	_buf = static_cast<char *>(realloc(_buf, _l));
	memset(_buf + _i, 0, _l - _i);
}

bool InputField::isSuitable(int ch)
{
	if(ch >= 'a' && ch <= 'z' ||
		ch >= 'A' && ch <= 'Z' ||
		ch >= '0' && ch <= '9')
		return true;

	const char *s = " `~!@#$%^&*()_+-={}|[]\\;:'\"/?.>,<";
	while(*s) if(*s++ == ch) return true;

	return false;
}

