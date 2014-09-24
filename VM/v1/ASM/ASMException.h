#ifndef ASMEXCEPTION_H
#define ASMEXCEPTION_H

#include <exception>
#include <string>

class ASMException : public std::exception
{
	public:
		ASMException(const char *, ...);
		ASMException(const std::string&);
		virtual ~ASMException( ) throw( );
		virtual const char *what( ) const throw( );
	private:
		std::string msg_;
};

#endif

