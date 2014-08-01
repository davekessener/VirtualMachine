#ifndef LIB_SDL_H
#define LIB_SDL_H

#include <functional>
#include "inc.h"

namespace sdl
{
	enum class Controls : size_t
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		ASCEND,
		DESCEND,
		ESCAPE,
		SPACE,
		SHIFT,
		RETURN
	};

	typedef std::function<void(int, int)> init_t;
	typedef std::function<bool(int)> update_t;
	typedef std::function<void(Controls, bool)> kb_input_t;
	typedef std::function<void(unsigned int, unsigned int, int, int)> m_input_t;

	void start(const char *, int, int);
	void set_init(init_t);
	void set_update(update_t);
	void set_input(kb_input_t, m_input_t);

	void trap_mouse(bool = true);
}

#endif

