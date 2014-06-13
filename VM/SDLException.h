#ifndef SDLEXCEPTION_H
#define SDLEXCEPTION_H

#include <exception>
#include <string>
#include <SDL2/SDL.h>

class SDLException : public std::exception
{
	public:
		SDLException( );
		SDLException(const char *, ...);
		SDLException(const std::string&);
		virtual ~SDLException( ) throw( );
		virtual const char *what( ) const throw( );
	private:
		std::string msg;
};

#endif

