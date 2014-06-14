#ifndef VM_ASM_INSTRUCTIONS_H
#define VM_ASM_INSTRUCTIONS_H

#include <iosfwd>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class Instructions
		{
			public:
				Instructions(std::istream&);
				~Instructions( ) throw();
			private:
		};
	}
}

#endif

