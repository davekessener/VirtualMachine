#ifndef VM_ASM_PREPROCESSOR_H
#define VM_ASM_PREPROCESSOR_H

#include <string>
#include "adef.h"
#include "Line.h"

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
				Line get( );
				bool ready( ) const;
				void clear( );
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

