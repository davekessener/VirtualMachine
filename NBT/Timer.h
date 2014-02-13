#ifndef TIMER_H
#define TIMER_H

#include <unistd.h>
#include <sys/timeb.h>
#include <ctime>
#include <ratio>
#include <chrono>

class Timer
{
	public:
		Timer( );
		~Timer( );
		void reset( );
		int elapsed( );
		void sleep(int);
	private:
		static long long curTimeMilliSec( );
		std::chrono::steady_clock::time_point _t;
};

#endif

