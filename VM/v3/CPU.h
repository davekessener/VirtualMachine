#ifndef DAV_VM_CPU_H
#define DAV_VM_CPU_H

#include <iostream>
#include "common.h"
#include "Periphery.h"

namespace vm
{
	void set_ram(size_t, const BYTE *, size_t);
	const BYTE *get_ram( );
	void reset( );
	void attach(Periphery_ptr);

	void run( );

	void print_reg(std::ostream&);
}

#endif

