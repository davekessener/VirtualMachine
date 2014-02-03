#include "Keyboard.h"

bool Keyboard::empty(void) 
{
	flush();

	return _buf.empty();
}

int Keyboard::peek(void)
{
	flush();

	return empty() ? -1 : _buf.at(0);
}

int Keyboard::poll(void)
{
	flush();

	if(empty()) return -1;

	int r = _buf.at(0);

	_buf.pop_front();

	return r;
}

int Keyboard::pop(void)
{
	flush();

	if(empty()) return -1;

	int r = _buf.at(_buf.size() - 1);

	_buf.pop_back();

	return r;
}

int Keyboard::operator[](int idx)
{
	flush();

	return idx < 0 || idx >= _buf.size() ? -1 : _buf.at(idx);
}

void Keyboard::clear(void)
{
	flush();

	_buf.clear();
}

int Keyboard::readSeq(int *b, int n)
{
	flush();

	int i = 0;

	if(!n) return 0;

	while(!empty())
	{
		*b++ = poll();
		if(++i == n) break;
	}

	return i;
}

Keyboard& Keyboard::instance(void)
{
	static Keyboard kb;

	return kb;
}

// # ---------------------------------------------------------------------------

Keyboard::Keyboard(void) : _buf()
{
	prepare_terminal();
}

Keyboard::~Keyboard(void)
{
	restore_terminal();
}

void Keyboard::flush(void)
{
	while(kbhit()) _buf.push_back(getch());
}

