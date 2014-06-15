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
				Tokenizer(const std::string&);
				~Tokenizer( ) throw();
				Tokenizer& operator>>(std::string&);
				Tokenizer& operator>>(Line&);
			private:
				const std::string str_;
				const char *cp_;
				int wc_;
		};
	}
}

#endif

