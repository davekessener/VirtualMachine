#include "Timer.h"

Timer::Timer(void) : _t(start())
{
}

Timer::~Timer(void)
{
}

void Timer::reset(void)
{
	_t = start();
}

int Timer::elapsed(void)
{
	int s = start() - _t;
	if(s < 0) s += 0x100000 * 1000;
	return s;
}

void Timer::sleep(int ms)
{
	usleep(ms * 1000);
}

int Timer::start(void)
{
	timeb tb;
	ftime(&tb);

	return tb.millitm + (tb.time & 0xfffff) * 1000;
}

