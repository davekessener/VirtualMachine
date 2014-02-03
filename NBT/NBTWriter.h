#ifndef NBT_WRITER_H
#define NBT_WRITER_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "NBT.h"
#include "gzstream.h"

namespace NBT
{
	class basic_nbt_writer
	{
		public:
			template<typename T>
			void write(T) const;
			void write(BYTE *d, size_t n) const
			{
				do_write(reinterpret_cast<char *>(d), n);
			}
		protected:
			virtual void do_write(char *, size_t) const = 0;
	};

	template<typename T>
	class nbt_writer : public basic_nbt_writer
	{
		public:
			nbt_writer(T& t) : os(t) { }
		protected:
			void do_write(char *, size_t) const;
		private:
			T &os;
	};

// # ---------------------------------------------------------------------------

	template<typename T>
	void basic_nbt_writer::write(T t) const
	{
#ifndef RETAIN_ENDIANNESS
		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
#endif

		do_write(reinterpret_cast<char *>(&t), sizeof(T));
	}

// # ---------------------------------------------------------------------------

#ifndef WRITER_IMP
	extern template class nbt_writer<std::ostream>;
	extern template class nbt_writer<gzip::ogzstream>;
#endif

	typedef nbt_writer<std::ostream> nbt_std_writer;
	typedef nbt_writer<gzip::ogzstream> nbt_gzip_writer;

	typedef basic_nbt_writer nbtostream;
}

#endif

