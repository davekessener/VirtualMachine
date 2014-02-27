#define WRITER_IMP
#include "Writer.h"
#undef WRITER_IMP

#define __WRITER_DEBUG

namespace NBT
{
	template<>
	void nbt_writer<std::ostream>::do_write(char *d, size_t n) const
	{
		assert(d);

		os.write(d, n);

#ifndef __WRITER_DEBUG
		BYTE *_b = new BYTE[n];
		memcpy(_b, d, n);
		fprintf(stderr, "%% Wrote (%d): ", (int) n);
		for(int i = 0 ; i < n ; ++i) fprintf(stderr, "%02hhx ", _b[i]);
		delete[] _b;
		std::cerr << std::endl;
#endif
	}

	template<>
	void nbt_writer<gzip::ogzstream>::do_write(char *d, size_t n) const
	{
		assert(d);

#ifndef __WRITER_DEBUG
		BYTE *_b = new BYTE[n];
		memcpy(_b, d, n);
		fprintf(stderr, "%% Wrote (%d): ", (int) n);
		for(int i = 0 ; i < n ; ++i) fprintf(stderr, "%02hhx ", _b[i]);
		delete[] _b;
		std::cerr << std::endl;
#endif

		while(n--) os << *d++;
	}

	template class nbt_writer<std::ostream>;
	template class nbt_writer<gzip::ogzstream>;
}

