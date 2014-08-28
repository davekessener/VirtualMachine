#include "AsyncIn.h"

AsyncIn::AsyncIn(void)
{
	cinfd->fd = fileno(stdin);
	cinfd->events = POLLIN;
}

bool AsyncIn::canRead(void)
{
	return poll(cinfd, 1, 8);
}

bool AsyncIn::pollCommand(std::string& s)
{
	if(canRead())
	{
		std::getline(std::cin, s);

		return true;
	}

	return false;
}

