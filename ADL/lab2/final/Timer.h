#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace dav
{
	class Timer
	{
		public:
			Timer( );
			~Timer( );
			void reset( );
			int elapsed(bool = false);
			int getDelta( );
			void keepRate(int);
			void sleep(int);
		private:
			static long long curTimeMilliSec( );
			std::chrono::steady_clock::time_point t_;
	};
}

#endif

