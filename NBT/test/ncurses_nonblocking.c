#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <iostream>
#include "Timer.h"

using namespace std;

class Banner
{
	public:
		Banner(const string& s, int x, int y) : _msg(s + "    "), _x(x), _y(y), _pos(0), _t(0) { }
		~Banner( ) { }
		void draw(void)
		{
			static int _o_pos = -1;

			if(_o_pos == _pos) return;

			_o_pos = _pos;

			string s = (_msg + _msg).substr(_pos, _msg.length());

			toScreen(s.c_str());
		}
		void toScreen(const char *s)
		{
			int x, y;

			getyx(stdscr, y, x);
			move(_y, _x);
			addstr(s);
			move(y, x);
		}
		void update(int ms)
		{
			_t += ms;
			while(_t > 200)
			{
				_t -= 200;
				if(++_pos == _msg.length()) _pos = 0;
			}
		}
		void setMsg(const string& s)
		{
			if(s.length() == 0) return;

			char *tmp = strdup(_msg.c_str());
			memset(tmp, ' ', _msg.length());
			toScreen(tmp);
			free(tmp);

			_msg = s + "    ";
			_pos = 0;
		}

	private:
		string _msg;
		int _x, _y, _t, _pos;
};

inline int isIn(int c, const char *s) { while(*s) if(c == *s++) return 1; return 0; }

int readAline(char *buf)
{
	static int i = -1, idx;
	static int x, y;

	if(i == -1)
	{
		i = 0;
		idx = 0;
		getyx(stdscr, y, x);
		curs_set(1);
		if(int l = strlen(buf)) memset(buf, '\0', l);
	}

	move(42, 0);
	clrtoeol();
	addstr(":");
	addstr(buf);
	move(42, idx + 1);

	int ch = getch();
	int l = strlen(buf);

	const char *el = " !*()-=+_;:'\"/?.>,<[]{}\\|@#$%^&`~";

	if(ch == ERR) return 0;

	switch(ch)
	{
		case KEY_LEFT:
			if(idx > 0) --idx;
			ch = 0;
			break;
		case KEY_RIGHT:
			if(idx < i) ++idx;
			ch = 0;
			break;
		case KEY_UP:
			idx = i;
			ch = 0;
			break;
		case KEY_DOWN:
			idx = 0;
			ch = 0;
			break;
		case KEY_BACKSPACE:
		case 0x7f:
			if(idx > 0)
			{
				int _i = idx - 1;
				while(_i++ < i) buf[_i - 1] = buf[_i];
				--idx;
				--i;
			}
			ch = 0;
			break;
		case '\e':
			buf[0] = '\0';
			goto end;
	}

	if(ch == '\n')
	{
		goto end;
	}
	else if(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || isIn(ch, el))
	{
		int _i = i;
		while(_i > idx) buf[_i] = buf[--_i];
		buf[idx++] = (char) ch;
		i++;
		addch((char)ch);
	}

	return 0;

end:
	
	i = -1;
	curs_set(0);
	move(42, 0);
	clrtoeol();
	move(y, x);

	return 1;
}

int main(void)
{
	Banner b("Hello, world!", 10, 3);
	Timer t;

	initscr();
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	timeout(0);
	clear();
	refresh();

	char buffer[256];
	bool querying = false;

	memset(buffer, '\0', 256);

	int ch;
	while(1)
	{
		int d = t.elapsed();
		t.reset();

		if(!querying) ch = getch();

		b.update(d);
		b.draw();

		if(ch == 'q') break;

		if(ch == ':' && !querying)
		{
			ch = 0;
			querying = true;
		}

		if(querying)
		{
			if(readAline(buffer))
			{
				querying = false;
				b.setMsg(buffer);
			}
		}

		refresh();

		d = 16 - (d + t.elapsed());
		if(d > 0) t.sleep(d);
	}

	endwin();

	return 0;
}

