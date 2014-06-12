#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <future>
#include <thread>
#include <ncurses.h>
#include "Timer.h"

using namespace std;

mutex mtx;

void ncurses_init(void)
{
	mtx.lock();
	initscr();
	keypad(stdscr, true);
	cbreak();
	curs_set(0);
	timeout(10);
	mtx.unlock();
}

void ncurses_drawAtXY(int x, int y, const string& s)
{
	mtx.lock();

	int _x, _y;
	getyx(stdscr, _y, _x);

	move(y, x);

	addstr(s.c_str());

	move(_y, _x);

	refresh();

	mtx.unlock();
}

void ncurses_readline(char *buf, bool &done)
{
	int x, y, i = 0;
	int ch;
	Timer t;

	mtx.lock();
	move(0, 0);
	addstr(" ");
	getyx(stdscr, y, x);
	move(42, 0);
	addstr(":");
	curs_set(1);
	mtx.unlock();

	while(1)
	{
		mtx.lock();
		echo();
		timeout(1);
		ch = getch();
		timeout(5);
		noecho();
		mtx.unlock();

		t.sleep(1);

		switch(ch)
		{
			case ERR:
				continue;
			case '\n':
				goto end;
			default:
				if(	ch >= 'a' && ch <= 'z' || 
					ch >= 'A' && ch <= 'Z' || 
					ch >= '0' && ch <= '9' || ch == ' ' || ch == '\'' || ch == ':') buf[i++] = (char) ch;
		}
	}

end:

	mtx.lock();

	buf[i] = '\0';
	done = true;
	move(42, 0);
	clrtoeol();
	move(y, x);
	curs_set(0);
	refresh();

	mtx.unlock();

	return;
}

void ncurses_refresh(void)
{
	mtx.lock();
	refresh();
	mtx.unlock();
}

struct Banner
{
	Banner(const string& s, int x, int y) : _msg(s + "    "), _x(x), _y(y), _pos(0), _t() { }
	void update( )
	{
		if(_t.elapsed() > 200)
		{
			_t.reset();
			if(++_pos == _msg.length()) _pos = 0;
		}
	}

	void draw( )
	{
		string s(_msg + _msg);

		ncurses_drawAtXY(_x, _y, s.substr(_pos, _msg.length()));
	}

	void setMsg(const string& s)
	{
		if(s.length() == 0) return;
		char *tmp = strdup(_msg.c_str());
		memset(tmp, ' ', _msg.length());
		_msg = string(tmp);
		draw();
		free(tmp);
		_msg = s + "    ";
		_pos = 0;
	}

	string _msg;
	int _x, _y;
	int _pos;
	Timer _t;
};

int main(void)
{
	Banner b(string("Hi there, handsome."), 10, 3);
	Timer timer;

	ncurses_init();

	future<void> f;
	bool done = false, running = false;
	char buf[256];

	int ch;
	while(true)
	{
		if(running)
		{
			timer.sleep(10);
		}
		else
		{
			mtx.lock();
			ch = getch();
			mtx.unlock();
		}

		if(ch == KEY_F(1)) break;

		if(done)
		{
			f.get();
			b.setMsg(string(buf));
			done = false;
			running = false;
		}

		b.update();
		b.draw();

		ncurses_refresh();

		if(ch == ':' && !running)
		{
			running = true;
			f = async(launch::async, [&buf, &done]()->void { ncurses_readline(buf, done); });
			ch = 0;
		}
	}

	endwin();

	return 0;
}

