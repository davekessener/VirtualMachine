#ifndef BANNER_H
#define BANNER_H

#include <iostream>
#include <cstring>
#include "Curse.h"
#include "Screen.h"
#include "Timer.h"

#define BANNER_STDDELAY 400
#define BANNER_MINSTRLEN 4

namespace ncurses
{
	class Banner
	{
		public:
			Banner(const std::string&, int, int, int = BANNER_STDDELAY, int = -1);
			~Banner( );
			void update( );
			void draw( );
			void setMsg(const std::string&);
		private:
			std::string _msg;
			int _x, _y;
			int _delay, _pos, _strlen;
			Timer _timer;
	};
}

#endif

