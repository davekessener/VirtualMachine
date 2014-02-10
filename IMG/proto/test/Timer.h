#ifndef TIMER_H
#define TIMER_H

#include <unistd.h>
#include <sys/timeb.h>

class Timer
{
	public:
		Timer( );
		~Timer( );
		void reset( );
		int elapsed( );
		void sleep(int);
	private:
		static int start( );
		int _t;
};

#endif

