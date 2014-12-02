#ifndef NBT_WRITER_H
#define NBT_WRITER_H

#include <iostream>
#include <algorithm>
#include "common.h"
#ifndef NBT_NO_GZ
#	include <dav/gzstream.h>
#endif

namespace nbt
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
#	ifndef NBT_NO_GZ
		extern template class nbt_writer<dav::gzip::ogzstream>;
#	endif
#endif

	typedef nbt_writer<std::ostream> nbt_std_writer;
#ifndef NBT_NO_GZ
	typedef nbt_writer<dav::gzip::ogzstream> nbt_gzip_writer;
#endif

	typedef basic_nbt_writer nbtostream;
}

#endif

