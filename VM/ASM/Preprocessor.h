#ifndef VM_ASM_PREPROCESSOR_H
#define VM_ASM_PREPROCESSOR_H

#include "adef.h"
#include <string>

namespace vm
{
	namespace assembler
	{
		class Preprocessor
		{
			public:
				Preprocessor( );
				~Preprocessor( ) throw();
				void feed(const std::string&);
				const std::string get( );
				bool ready( ) const;
				void clear( );
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

