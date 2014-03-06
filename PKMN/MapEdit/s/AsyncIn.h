#ifndef MAP_ASYNCIN_H
#define MAP_ASYNCIN_H

#include <iostream>
#include <sys/poll.h>

class AsyncIn
{
	public:
		AsyncIn( );
		bool pollCommand(std::string&);
	private:
		bool canRead( );
		pollfd cinfd[1];
};

#endif

