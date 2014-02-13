#ifndef CURSE_H
#define CURSE_H

#include <functional>
#include <cstdint>
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
			static void play(Curse&);
		protected:
			virtual void init( );
			virtual void input(int) = 0;
			virtual void update(int) = 0;
			virtual void refresh( ) = 0;
			virtual void finalize( );
			void setInputFunction(inputFn);
			void quit( );
		private:
			void run( );
			void setDefaultInput( );
			bool running;
			Timer timer;
			inputFn _input;
	};
}

#endif

