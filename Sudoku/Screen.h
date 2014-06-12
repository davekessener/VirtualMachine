#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <sstream>
#include <wchar.h>
#include <locale.h>

class Screen
{
	public:
		void clear( );
		void setPos(int, int);
		void flush( );
		int width( );
		int height( );
		void eraseLine( );
		void puts(const char *);
		void puts(const wchar_t *);
		template<typename T> Screen& operator<<(const T&);
		static Screen& instance( );
	private:
		Screen( );
		Screen(const Screen&);
		Screen(Screen&&);
		~Screen( );
		Screen& operator=(const Screen&);
		Screen& operator=(Screen&&);
};

template<typename T>
Screen& Screen::operator<<(const T& t)
{
	std::stringstream ss("", std::ios::app | std::ios::out);

	ss << t;

	puts(ss.str().c_str());

	return *this;
}

#endif

