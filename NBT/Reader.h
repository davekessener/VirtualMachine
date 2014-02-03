#ifndef NBT_READER_H
#define NBT_READER_H

#include <iostream>
#include <fstream>
#include <assert.h>
#include "gzstream.h"
#include "NBT.h"

namespace NBT
{
	class basic_nbt_reader
	{
		public:
			template<typename T>
			T read();
			basic_nbt_reader& read(BYTE *d, size_t n)
			{
				do_read(d, n);
			}
		protected:
			virtual void do_read(char *, size_t) = 0;
	};

	template<typename T>
	T basic_nbt_reader::read(void)
	{
		T t;
		do_read(reinterpret_cast<char *>(&t), sizeof(T));

#ifndef RETAIN_ENDIANNESS
		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
#endif

		return t;
	}

	template<typename T>
	class nbt_reader : public basic_nbt_reader
	{
		public:
			nbt_reader(T& t) : is(t) { }
		protected:
			void do_read(char *, size_t);
		private:
			T &is;
	};

	template<>
	void nbt_reader<std::istream>::do_read(char *d, size_t n)
	{
		assert(d);

		is.read(d, n);
	}

	template<>
	void nbt_reader<gzip::igzstream>::do_read(char *d, size_t n)
	{
		assert(d);

		while(n--) is >> *d++;
	}

	typedef nbt_reader<std::istream> nbt_std_reader;
	typedef nbt_reader<gzip::igzstream> nbt_gzip_reader;
}

#endif

