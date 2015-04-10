#define WRITER_IMP
#include "Writer.h"
#undef WRITER_IMP
#include <fstream>
#include <cassert>
#include <cstdio>
#include <cstring>

#ifndef NBT_NO_GZ
using namespace dav;
#endif

namespace nbt
{
	template<>
	void nbt_writer<std::ostream>::do_write(char *d, size_t n) const
	{
		assert(d);

		os.write(d, n);

#ifdef DEBUG
		BYTE *_b = new BYTE[n];
		memcpy(_b, d, n);
		fprintf(stderr, "%% Wrote (%d): ", (int) n);
		for(int i = 0 ; i < n ; ++i) fprintf(stderr, "%02hhx ", _b[i]);
		delete[] _b;
		std::cerr << std::endl;
#endif
	}

#ifndef NBT_NO_GZ
	template<>
	void nbt_writer<gzip::ogzstream>::do_write(char *d, size_t n) const
	{
		assert(d);

#	ifdef DEBUG
		BYTE *_b = new BYTE[n];
		memcpy(_b, d, n);
		fprintf(stderr, "%% Wrote (%d): ", (int) n);
		for(int i = 0 ; i < n ; ++i) fprintf(stderr, "%02hhx ", _b[i]);
		delete[] _b;
		std::cerr << std::endl;
#	endif

		while(n--) os << *d++;
	}
#endif

	template class nbt_writer<std::ostream>;
#ifndef NBT_NO_GZ
	template class nbt_writer<gzip::ogzstream>;
#endif
}

