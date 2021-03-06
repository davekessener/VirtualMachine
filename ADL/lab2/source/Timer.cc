#include "Timer.h"
#include <unistd.h>
#include <sys/timeb.h>
#include <ctime>
#include <ratio>

namespace dav
{
	Timer::Timer(void) : t_(std::chrono::steady_clock::now())
	{
	}
	
	Timer::~Timer(void)
	{
	}
	
	void Timer::reset(void)
	{
		t_ = std::chrono::steady_clock::now();
	}
	
	int Timer::elapsed(bool micro)
	{
		typedef std::ratio<1, 1000000> ratio;
		typedef std::chrono::duration<long long, ratio> duration;
		duration span = std::chrono::duration_cast<duration>
				(std::chrono::steady_clock::now() - t_);
	
		auto c = span.count();
		return micro ? c : (c / 1000);
	}
	
	int Timer::getDelta(void)
	{
		int r = elapsed();
		reset();
		return r;
	}
	
	void Timer::keepRate(int fr)
	{
		if(fr > 0) sleep((1000 / fr) - getDelta());
	}
	
	void Timer::sleep(int ms)
	{
		if(ms > 0) usleep(ms * 1000);
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
}

