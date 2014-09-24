#ifndef VM_ASM_INSTRUCTIONS_H
#define VM_ASM_INSTRUCTIONS_H

#include <iosfwd>
#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct Opcode;
		struct Line;

		class Instructions
		{
			public:
				Instructions(std::istream&);
				~Instructions( ) throw();
				Opcode *translate(const Line&) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

