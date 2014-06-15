#ifndef VM_ASM_SYMTABLE_H
#define VM_ASM_SYMTABLE_H

#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class SymTable
		{
			public:
				SymTable( );
				~SymTable( ) throw();
				void insert(const std::string&, int);
				void clear( );
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

