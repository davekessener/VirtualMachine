#ifndef VM_ASM_ASSEMBLER_H
#define VM_ASM_ASSEMBLER_H

#include "adef.h"
#include <iosfwd>
#include <string>

namespace vm
{
	namespace assembler
	{
		class Reader;

		class Assembler
		{
			public:
				Assembler( );
				~Assembler( ) throw();
				void assemble(Reader&);
				void out(std::ostream&);
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

