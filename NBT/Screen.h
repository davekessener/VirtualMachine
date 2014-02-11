#ifndef CURSE_SCREEN_H
#define CURSE_SCREEN_H

#include "Curse.h"

#ifdef CURSE_SCREEN_MAIN
#include <cwchar>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#endif

namespace ncurses
{
	class Screen
	{
		public:
			void printf(const char *, ...);
			void printf(const wchar_t *, ...);
			void refresh( );
			void move(int, int);
			void eraseLine(int = -1);
			int getCursorX( );
			int getCursorY( );
			int getScreenWidth( );
			int getScreenHeight( );
			static Screen& instance( );
		private:
			Screen( );
			~Screen( );
			Screen(const Screen&);
			Screen& operator=(const Screen&);
			Curse &curse;
	};
}

#endif

