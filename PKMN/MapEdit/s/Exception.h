#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>
#include <exception>

class Exception : public std::exception
{
	public:
		Exception(const std::string& s) : msg(s) { }
		virtual ~Exception( ) throw() { }
		virtual const char *what( ) const throw() { return msg.c_str(); }
	private:
		std::string msg;
};

#endif

