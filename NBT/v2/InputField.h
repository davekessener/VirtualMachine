#ifndef INPUTFIELD_H
#define INPUTFIELD_H

#include <iostream>
#include <functional>
#include "Display.h"

#ifdef INPUTFIELD_MAIN
#include <cstring>

#define BUF_SZ 128
#endif

class InputField
{
	public:
		InputField();
		~InputField( );
		void start(int, int, int, const char * = NULL);
		bool isRunning( );
		void clean( );
		void input(int);
		std::pair<int, int> getCurPos( );
		std::pair<int, int> getPosition( );
		std::string getValue( );
		std::string getVisible( );
	private:
		void resize(int = -1);
		bool isSuitable(int);
	private:
		int x_, y_, l_, pos_;
		char *buf_;
		int i_, idx_, len_;
};

#endif

