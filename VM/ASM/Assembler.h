#ifndef VM_ASM_ASSEMBLER_H
#define VM_ASM_ASSEMBLER_H

#include <iosfwd>
#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class Assembler
		{
			public:
				Assembler(std::istream&);
				~Assembler( ) throw();
				void assemble(const std::string&);
				void out(std::ostream&);
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

