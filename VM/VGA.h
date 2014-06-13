#ifndef VM_VGA_H
#define VM_VGA_H

#include "ddef.h"
#include "Periphery.h"
#include "Image.h"
#include "Timer.h"
#include <deque>

namespace vm
{
	namespace periphery
	{
		class BasicVGA : public Periphery
		{
			static const size_t COLS = 120, ROWS = 46;
			static const size_t SCREEN_SIZE = COLS * ROWS;
			static const size_t CHAR_SIZE = 8;
			static const int BLINK_STEP = 500;

			public:
				BasicVGA( );
				void in(WORD);
				WORD out( );
				void step( );
				void link(cpu::CPU&);
			private:
				void loadFrom(ADDRESS);
				void refresh(bool = false);
			private:
				std::deque<WORD> queue_;
				WORD buffer_[SCREEN_SIZE];
				struct { WORD x, y; bool active, show, show_old; } cursor_;
				sdl::Image tex_;
				sdl::color_rgba_t palette_[16];
				const WORD *vidmem_;
				Timer t_;
		};
	}
}

#endif

