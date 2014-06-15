#ifndef VM_ASM_READER_H
#define VM_ASM_READER_H

#include <string>
#include "adef.h"
#include "Line.h"

namespace vm
{
	namespace assembler
	{
		class Reader
		{
			public:
				Reader(const std::string&);
				~Reader( ) throw();
				const Line getline( );
				void ungetline(const Line&);
				bool empty( );
				int getCurrentLineNumber( ) const;
			private:
				struct Impl;
				Impl *impl_;

			private:
				Reader(const Reader&);
				Reader& operator=(const Reader&);
		};
	}
}

#endif

