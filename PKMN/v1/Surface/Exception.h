#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>
#include <exception>
#include "../Misc/Logger.h"

class Exception : public std::exception
{
	public:
		Exception(const std::string& s) : msg(s) { Logger::log("[FATAL] '%s'", s.c_str()); }
		virtual ~Exception( ) throw() { }
		virtual const char *what( ) const throw() { return msg.c_str(); }
	private:
		std::string msg;
};

#endif

