#ifndef CURSE_H
#define CURSE_H

#include <deque>
#include "Display.h"
#include "Terminal.h"

namespace ncurses
{
	class Curse : public display::Terminal
	{
		public:
			Curse( );
			~Curse( );
			void init( );
			void finalize( );
			void flush( );
			void getCursorPos(int&, int&);
			void setCursorPos(int, int);
			void getScreenSize(int&, int&);
			void showCursor(bool = true);
			void eraseScreen( );
			void eraseLine( );
			int getChar( );
			void pause( );
			void printf(const char *, va_list);
			void printfw(const wchar_t *, va_list);
			void saveCursorPos( );
			void restoreCursorPos( );
		private:
			std::deque<std::pair<int, int>> cursorBackup;
	};
}

#endif

