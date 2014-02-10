#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>
#include <deque>
#include "conio_e.h"

#define KEY_CTRL_C 3
#define KEY_CTRL_D 4
#define KEY_ESC 27

class Keyboard
{
	public:
		bool empty( );
		int peek( );
		int poll( );
		int pop( );
		int operator[](int);
		void clear( );
		int readSeq(int *, int = -1);
		static Keyboard& instance( );
	private:
		Keyboard( );
		Keyboard(const Keyboard&);
		~Keyboard( );
		Keyboard& operator=(const Keyboard&);

		void flush( );
		std::deque<int> _buf;
};

#endif

