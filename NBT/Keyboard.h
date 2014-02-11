#ifndef CURSE_KEYBOARD_H
#define CURSE_KEYBOARD_H

#include <iostream>
#include "Curse.h"
#include "Screen.h"

#ifdef CURSE_KEYBOARD_MAIN
#endif

namespace ncurses
{
	class Keyboard
	{
		public:
			void pause( );
			std::string getline( );
			std::string readCommand( );
			static Keyboard& instance( );
		private:
			Keyboard( );
			Keyboard(const Keyboard&);
			~Keyboard( );
			Keyboard& operator=(const Keyboard&);
			Curse &curse;
	};
}

#endif

