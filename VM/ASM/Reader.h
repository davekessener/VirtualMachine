#ifndef VM_ASM_READER_H
#define VM_ASM_READER_H

#include <fstream>
#include <deque>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class Reader
		{
			public:
				Reader(std::istream&);
				~Reader( ) throw();
				const std::string getline( );
				void ungetline(const std::string&);
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

