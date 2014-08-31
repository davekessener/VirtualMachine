#ifndef PKMN_KEYPAD_H
#define PKMN_KEYPAD_H

#include "common.h"
#include <vector>
#include <set>
#include <map>
#include <dav/sdl.h>

namespace pkmn
{
	enum class Key : DWORD
	{
		DOWN,
		UP,
		LEFT,
		RIGHT,
		A,
		B,
		L,
		R,
		START,
		SELECT,
		NONE
	};

	class Keypad
	{
		public:
		typedef dav::sdl::Controls Controls;

		public:
			static inline bool isPressed(Key key) { return instance().doIsPressed(key); }
			static inline void press(Controls key, bool pressed) { instance().doPress(key, pressed); }
			static inline Key recent( ) { return instance().doRecent(); }
		private:
			Keypad( );
			~Keypad( ) = default;
		private:
			bool doIsPressed(Key) const;
			void doPress(Controls, bool);
			Key doRecent( ) const;
		private:
			std::set<Controls> keys_;
			std::map<Key, Controls> lookup_;
			std::map<Controls, Key> rlook_;
			std::vector<Key> history_;
		private:
			static Keypad& instance( ) { static Keypad kp; return kp; }
		private:
			Keypad(const Keypad&) = delete;
			Keypad& operator=(const Keypad&) = delete;
	};
}

#endif

