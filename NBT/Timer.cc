#include "Timer.h"

Timer::Timer(void) : _t(std::chrono::steady_clock::now())
{
}

Timer::~Timer(void)
{
}

void Timer::reset(void)
{
	_t = std::chrono::steady_clock::now();
}

int Timer::elapsed(void)
{
	std::chrono::duration<long long, std::ratio<1, 1000>> span = 
		std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1, 1000>>>(std::chrono::steady_clock::now() - _t);

	return span.count();
}

int Timer::getDelta(void)
{
	int r = elapsed();
	reset();
	return r;
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
//	long long s = curTimeMilliSec() - _t;
//	if(s < 0) s += 0x100000 * 1000;
//	return s;
}

