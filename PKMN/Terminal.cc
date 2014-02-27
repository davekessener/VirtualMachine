#define TERMINAL_MAIN
#include "Terminal.h"
#undef TERMINAL_MAIN

namespace display
{
	Terminal *Terminal::terminal = NULL;

	Terminal& Terminal::instance(void)
	{
		assert(terminal);

		return *terminal;
	}
}

