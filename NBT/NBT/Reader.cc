#define READER_IMP
#include "NBTReader.h"
#undef READER_IMP

#define __READER_DEBUG

namespace NBT
{
	template<>
	void nbt_reader<std::istream>::do_read(char *d, size_t n) const
	{
		assert(d);

		is.read(d, n);

#ifndef __READER_DEBUG
		QWORD qw = 0; memcpy(&qw, d, n); std::reverse(static_cast<BYTE *>(&qw), static_cast<BYTE *>(&qw) + n);
		fprintf(stderr, "%% Read (%d): 0x%0*lx\n", (int) n, 2 * (int) n, qw);
#endif
	}

	template<>
	void nbt_reader<gzip::igzstream>::do_read(char *d, size_t n) const
	{
		assert(d);

		char *_d = d;
		int _n = (int) n;

		while(n--) is >> *d++;

#ifndef __READER_DEBUG
		QWORD qw = 0; memcpy(&qw, _d, _n); std::reverse(static_cast<BYTE *>(&qw), static_cast<BYTE *>(&qw) + _n);
		fprintf(stderr, "%% Read (%d): 0x%0*lx\n", _n, 2 * _n, qw);
#endif
	}
	
	template class nbt_reader<std::istream>;
	template class nbt_reader<gzip::igzstream>;
}

