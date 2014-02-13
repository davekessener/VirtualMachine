#include "Timer.h"

Timer::Timer(void) : _t(curTimeMilliSec())
{
}

Timer::~Timer(void)
{
}

void Timer::reset(void)
{
	_t = curTimeMilliSec();
}

int Timer::elapsed(void)
{
	long long s = curTimeMilliSec() - _t;
	if(s < 0) s += 0x100000 * 1000;
	return s;
}

void Timer::sleep(int ms)
{
	usleep(ms * 1000);
}

long long Timer::curTimeMilliSec(void)
{
	timeb tb;
	ftime(&tb);

	return tb.millitm + (tb.time & 0xfffff) * 1000;
}

