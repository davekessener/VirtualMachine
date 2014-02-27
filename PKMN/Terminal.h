#ifndef TERMINAL_H
#define TERMINAL_H

#include <cstdarg>
#include <cassert>
#include "Display.h"

#ifdef TERMINAL_MAIN
#endif

namespace display
{
	class Terminal
	{
		public:
			template<typename T>
				static void create( );
			static Terminal& instance( );
			virtual void init( ) = 0;
			virtual void finalize( ) = 0;
			virtual void flush( ) = 0;
			virtual void getCursorPos(int&, int&) = 0;
			virtual void setCursorPos(int, int) = 0;
			virtual void getScreenSize(int&, int&) = 0;
			virtual void showCursor(bool) = 0;
			virtual void eraseScreen( ) = 0;
			virtual void eraseLine( ) = 0;
			virtual int getChar( ) = 0;
			virtual void pause( ) = 0;
			virtual void printf(const char *, va_list) = 0;
			virtual void printfw(const wchar_t *, va_list) = 0;
			inline int getCursorX( ) { int x, y; getCursorPos(x, y); return x; }
			inline int getCursorY( ) { int x, y; getCursorPos(x, y); return y; }
			inline int getScreenWidth( ) { int w, h; getScreenSize(w, h); return w; }
			inline int getScreenHeight( ) { int w, h; getScreenSize(w, h); return h; }
			inline void printf(const char *s, ...)
			{
				va_list l;
				va_start(l, s);
				printf(s, l);
				va_end(l);
			}
			inline void printfw(const wchar_t *s, ...)
			{
				va_list l;
				va_start(l, s);
				printfw(s, l);
				va_end(l);
			}
			inline void saveCursorPos(bool restore = false)
			{
				static int x, y;
				if(restore) setCursorPos(x, y); else getCursorPos(x, y);
			}
			inline void restoreCursorPos( ) { saveCursorPos(true); }
		protected:
			virtual ~Terminal( ) { }
		private:
			static Terminal *terminal;
	};

	template<typename T>
	void Terminal::create(void)
	{
		static T t;

		assert(terminal == NULL);

		terminal = static_cast<Terminal *>(&t);
	}
}

#endif

