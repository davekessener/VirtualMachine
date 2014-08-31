#ifndef LIB_SDL_H
#define LIB_SDL_H

#include <functional>
#include <string>
#include "inc.h"

namespace dav
{
	namespace sdl
	{
		enum class Controls : DWORD
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			ESCAPE,
			SHIFT,
			CTRL,
			BACKSPACE,
			DELETE,
			TAB = '\t',
			RETURN = '\n',
			SPACE = ' ',
			DASH = '-',
			EQUALS = '=',
			LSQUARE = '[',
			RSQUARE = ']',
			QUOTE = '\'',
			SEMICOLON = ';',
			BACKSLASH = '\\',
			COMMA = ',',
			PERIOD = '.',
			SLASH = '/',
			BACKTICK = '`',
			A = 'a', B, C, D, E, F, G, H,
			I, J, K, L, M, N, O, P,
			Q, R, S, T, U, V, W, X,
			Y, Z,
			ZERO = '0', ONE, TWO, THREE, FOUR,
			FIVE, SIX, SEVEN, EIGHT, NINE
		};

		enum class MouseButtons : DWORD
		{
			LEFT,
			RIGHT,
			MIDDLE
		};
	
		char translate(Controls, bool = false);
	
		typedef std::function<void(int, int)> init_t;
		typedef std::function<bool(int)> update_t;
		typedef std::function<void(Controls, bool)> kb_input_t;
		typedef std::function<void(uint, uint, int, int)> mm_input_t;
		typedef std::function<void(MouseButtons, uint, uint, bool)> m_input_t;
		typedef std::function<void(int, int)> mw_input_t;
	
		void start(const std::string&, int, int, bool = false);
		void set_init(init_t);
		void set_update(update_t);
		void set_input(kb_input_t);
		void set_mouse(m_input_t, mm_input_t = nullptr, mw_input_t = nullptr);
	
		void trap_mouse(bool = true);
	}
}

#endif

