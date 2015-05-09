#ifndef DAV_VM_ASM_H
#define DAV_VM_ASM_H

#include "common.h"
#include <dav/io.hpp>
#include <memory>

namespace vm
{
	using CharExtract_t = dav::io::ReadContainer<char>;

	class Assembler
	{
		public:
			~Assembler( );
			template<typename I>
				void parse(I i1, I i2)
					{ parse(dav::io::IStreamContainer<char>(i1, i2)); }
			void parse(CharExtract_t&& rv) { parse(rv); }
			void parse(CharExtract_t&);
		private:
			struct Impl;
			std::unique_ptr<Impl> impl_;
	};
}

#endif

