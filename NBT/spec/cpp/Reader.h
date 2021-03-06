#ifndef NBT_READER_H
#define NBT_READER_H

#include <iostream>
#include <algorithm>
#include "common.h"
#ifndef NBT_NO_GZ
#	include <dav/gzstream.h>
#endif

namespace nbt
{
	class basic_nbt_reader
	{
		public:
			template<typename T>
			T read() const;
			void read(BYTE *d, size_t n) const
			{
				do_read(reinterpret_cast<char *>(d), n);
			}
		protected:
			virtual void do_read(char *, size_t) const = 0;
	};

	template<typename T>
	class nbt_reader : public basic_nbt_reader
	{
		public:
			nbt_reader(T& t) : is(t) { }
		protected:
			void do_read(char *, size_t) const;
		private:
			T &is;
	};

// # ---------------------------------------------------------------------------

	template<typename T>
	T basic_nbt_reader::read(void) const
	{
		T t;
		do_read(reinterpret_cast<char *>(&t), sizeof(T));

#ifndef RETAIN_ENDIANNESS
		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
#endif

		return t;
	}

// # ---------------------------------------------------------------------------

#ifndef READER_IMP
	extern template class nbt_reader<std::istream>;
#	ifndef NBT_NO_GZ
		extern template class nbt_reader<dav::gzip::igzstream>;
#	endif
#endif

	typedef nbt_reader<std::istream> nbt_std_reader;
#ifndef NBT_NO_GZ
	typedef nbt_reader<dav::gzip::igzstream> nbt_gzip_reader;
#endif

	typedef basic_nbt_reader nbtistream;
}

#endif

