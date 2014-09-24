#ifndef VM_ASM_PREPROCESSOR_H
#define VM_ASM_PREPROCESSOR_H

#include <string>
#include "adef.h"
#include "Line.h"

namespace vm
{
	namespace assembler
	{
		struct Tokenizer;
		struct SymTable;

		class Preprocessor
		{
			public:
				Preprocessor( );
				~Preprocessor( ) throw();
				void feed(const Tokenizer&);
				Line get( );
				bool ready( ) const;
				void clear( );
				void swap(Preprocessor&) throw();
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

