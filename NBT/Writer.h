#ifndef NBT_WRITER_H
#define NBT_WRITER_H

namespace NBT
{
	class basic_nbt_writer
	{
		public:
			template<typename T>
			void write(T);
			basic_nbt_writer& write(BYTE *d, size_t n)
			{
				do_write(d, n);
			}
		protected:
			virtual void do_write(char *, size_t) = 0;
	};

	template<typename T>
	void basic_nbt_writer::write(T t)
	{
#ifndef RETAIN_ENDIANNESS
		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
#endif

		do_write(reinterpret_cast<char *>(&t), sizeof(T));
	}

	template<typename T>
	class nbt_writer : public basic_nbt_writer
	{
		public:
			nbt_writer(T& t) : os(t) { }
		protected:
			void do_write(char *, size_t);
		private:
			T &os;
	};

	template<>
	void nbt_writer<std::ostream>::do_write(char *d, size_t n)
	{
		assert(d);

		os.write(d, n);
	}

	template<>
	void nbt_writer<gzip::ogzstream>::do_write(char *d, size_t n)
	{
		assert(d);

		while(n--) os << *d++;
	}

	typedef nbt_writer<std::ostream> nbt_std_writer;
	typedef nbt_writer<gzip::ogzstream> nbt_gzip_writer;
}

#endif

