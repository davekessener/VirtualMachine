#include "Banner.h"

namespace ncurses
{
	Banner::Banner(const std::string& msg, int x, int y, int delay, int len) : 
		_msg(msg), _x(x), _y(y), _delay(delay), _pos(0), _timer()
	{
		std::string tmp;
		for(int i = 0 ; i < BANNER_MINSTRLEN ; ++i) tmp += ' ';
		_msg += tmp;
		_strlen = len > 0 ? len : _msg.length();
	}

	Banner::~Banner(void)
	{
	}

	void Banner::update(void)
	{
		if(_timer.elapsed() > _delay)
		{
			_timer.reset();
			if(++_pos == _msg.length()) _pos = 0;
		}
	}

	void Banner::draw(void)
	{
		std::string out(_msg);

		while(out.length() - _pos <= _strlen) out += _msg;

		out = out.substr(_pos, _strlen);

		Curse::Position pos = Curse::instance().getCursorPosition();
		Screen::instance().move(_x, _y);
		Screen::instance().printf(out.c_str());
		Curse::instance().moveCursor(pos.x, pos.y);
	}

	void Banner::setMsg(const std::string& msg)
	{
		std::string tmp = msg + _msg.substr(_msg.length() - BANNER_MINSTRLEN, BANNER_MINSTRLEN);
		if(tmp.length() > 0) _msg = tmp;
	}
}

