#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <sstream>
#include "size.h"

class Screen
{
	public:
		Screen(std::ostream& = std::cout);
		Screen(int, int, std::ostream& = std::cout);
		Screen(const Screen&);
		Screen(Screen&&);
		~Screen( );
		void clear( );
		void setPos(int, int);
		void setColor(int, int = -1, int = -1);
		void resetColors( );
		template<typename T> void out(const T&);
		template<typename T> Screen& operator<<(const T& t);
		void put(int, int, char);
		void flush( );
		int width( );
		int height( );
		bool needsRedraw( );
		void eraseLine( );
	private:
		Screen& operator=(const Screen&);
		Screen& operator=(Screen&&);

		int _width, _height;
		std::ostream &_os;
		std::stringstream buffer;
};

template<typename T>
void Screen::out(const T& t)
{
	buffer << t;
}

template<typename T>
Screen& Screen::operator<<(const T& t)
{
	buffer << t;

	return *this;
}

#endif

