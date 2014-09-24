#ifndef VM_ASM_TOKENIZER_H
#define VM_ASM_TOKENIZER_H

#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct Line;

		class Tokenizer
		{
			public:
				Tokenizer( );
				Tokenizer(Tokenizer&&);
				Tokenizer(const Tokenizer&);
				explicit Tokenizer(const std::string&);
				~Tokenizer( ) throw();
				Tokenizer& operator=(Tokenizer&&);
				Tokenizer& operator=(const Tokenizer&);
				Tokenizer& operator>>(Line&);
				Line getline( );
				const std::string& filename( ) const;
				void swap(Tokenizer&) throw();
				bool ready( ) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

