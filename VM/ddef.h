#ifndef VM_DDEF_H
#define VM_DDEF_H

#include <cstdint>
#include <cstddef>

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
typedef decltype(NULL) NULL_t;

namespace vm
{
	typedef std::uint8_t BYTE;

	typedef std::uint16_t WORD;

	typedef WORD OFFSET;
	typedef WORD ADDRESS;

	static const ADDRESS VIDEO_MEMORY = 0xc000;
	static const ADDRESS INT_TABLE = 0x0100;

	static const int INT_CLOCK = 0;
	static const int INT_KEYBOARD = 1;
}

#endif

