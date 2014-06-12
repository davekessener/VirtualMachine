#define NBTEXCEPTION_MAIN
#include "NBTException.h"
#undef NBTEXCEPTION_MAIN

namespace nbt
{
	NBTException::NBTException(const std::string& _msg) : exception(), msg(_msg)
	{
	}
	
	NBTException::~NBTException(void) throw()
	{
	}
	
	const char *NBTException::what(void) const throw()
	{
		return msg.c_str();
	}
}

