#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include "size.h"

class Screen
{
	public:
		Screen(std::ostream& = std::cout);
		Screen(int, int, std::ostream& = std::cout);
		Screen(const Screen&);
		Screen(Screen&&);
		~Screen( );
		void clear( ) const;
		void setPos(int, int) const;
		void setColor(int, int = -1, int = -1) const;
		void resetColors( ) const;
		template<typename T> void out(const T&) const;
		template<typename T> Screen& operator<<(const T& t);
		template<typename T> const Screen& operator<<(const T& t) const;
		void put(int, int, char) const;
		void flush( ) const;
		int width( ) const;
		int height( ) const;
	private:
		Screen& operator=(const Screen&);
		Screen& operator=(Screen&&);

		int _width, _height;
		std::ostream &_os;
};

template<typename T>
void Screen::out(const T& t) const
{
	_os << t;
}

template<typename T>
Screen& Screen::operator<<(const T& t)
{
	_os << t;

	return *this;
}

template<typename T>
const Screen& Screen::operator<<(const T& t) const
{
	_os << t;

	return *this;
}

#endif

