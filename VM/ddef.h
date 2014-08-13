#ifndef VM_DDEF_H
#define VM_DDEF_H

#include <dav/inc.h>

namespace vm
{
	typedef WORD OFFSET;
	typedef WORD ADDRESS;

	static const ADDRESS VIDEO_MEMORY = 0xc000;
	static const ADDRESS INT_TABLE = 0x0100;

	static const int INT_CLOCK = 0;
	static const int INT_KEYBOARD = 1;

	namespace vga
	{
		static const size_t COLS = 120, ROWS = 46;
		static const size_t SCREEN_SIZE = COLS * ROWS;
		static const size_t CHAR_SIZE = 8;
		static const int BLINK_STEP = 500;
	}
}

#endif

