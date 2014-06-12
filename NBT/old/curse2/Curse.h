#ifndef CURSE_H
#define CURSE_H

#include <functional>
#include <cstdint>
#include <cstring>
#include "CurseBase.h"
#include "Timer.h"

#ifdef CURSE_MAIN
#include <locale.h>
#endif

namespace ncurses
{
	class Curse : public Curse_base
	{
		typedef Curse_base CurseBase;
		typedef std::function<bool(int)> inputFn;

		public:
			Curse( );
			~Curse( );
			static void play(Curse *);
		protected:
			virtual void init( );
			virtual void input(int) = 0;
			virtual void update(int) = 0;
			virtual void refresh( ) = 0;
			virtual void finalize( );
			void setInputFunction(inputFn);
			void quit( );
			template<Borders B>
			void drawBorder(int, int, int, int);
			void drawBorder(int, int, int, int, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t);
		private:
			void run( );
			void setDefaultInput( );
			bool running;
			Timer timer;
			inputFn _input;
	};

	template<Borders B>
	void Curse::drawBorder(int x1, int y1, int x2, int y2)
	{
		drawBorder(x1, y1, x2, y2, 
			BorderTypes<B>::SIDES_HORIZONTAL,
			BorderTypes<B>::SIDES_VERTICAL,
			BorderTypes<B>::CORNER_TOPLEFT,
			BorderTypes<B>::CORNER_TOPRIGHT,
			BorderTypes<B>::CORNER_BOTTOMLEFT,
			BorderTypes<B>::CORNER_BOTTOMRIGHT);
	}
}

#endif

