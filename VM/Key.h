#ifndef VM_KEY_H
#define VM_KEY_H

#include <cstdint>
#include "ddef.h"

namespace vm
{
	namespace sdl
	{
		enum class SpecialKey
		{
			NOT,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

		class Key
		{
			public:
				Key(std::uint32_t);
				explicit operator char( ) const;
				bool isSpecial( ) const;
				SpecialKey get( ) const;
			private:
				std::uint32_t v_;
				char c_;
				SpecialKey s_;
		};
	}
}

#endif

