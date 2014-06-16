#ifndef VM_ASM_READER_H
#define VM_ASM_READER_H

#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class Reader
		{
			public:
				Reader( );
				Reader(Reader&&);
				Reader(const Reader&);
				~Reader( ) throw();
				Reader& operator=(Reader&&);
				Reader& operator=(const Reader&);
				void swap(Reader&) throw();
			private:
				struct Impl;
				Impl *impl_;
				size_t *ref_;
		};
	}
}

#endif

