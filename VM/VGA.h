#ifndef VM_VGA_H
#define VM_VGA_H

#include <deque>
#include "ddef.h"
#include "Periphery.h"
//#include "Image.h"
#include <dav/Timer.h>

namespace vm
{
	namespace periphery
	{
		class BasicVGA : public Periphery
		{
			public:
				BasicVGA( );
				void in(WORD);
				WORD out( );
				void step( );
				void link(cpu::CPU&);
			private:
				void loadFrom(ADDRESS);
				void refresh( ) const;
			private:
				std::deque<WORD> queue_;
//				WORD buffer_[vga::SCREEN_SIZE];
				struct { WORD x, y; bool active, show, show_old; } cursor_;
//				sdl::Image tex_;
				DWORD tex_;
//				sdl::color_rgba_t palette_[16];
				DWORD palette_[16];
				DWORD ipalette_[16];
				const WORD *vidmem_;
				dav::Timer t_;
		};
	}
}

#endif

