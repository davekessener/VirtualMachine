#define READER_IMP
#include "Reader.h"
#undef READER_IMP
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
	void nbt_reader<std::istream>::do_read(char *d, size_t n) const
	{
		assert(d);

		is.read(d, n);

#ifdef DEBUG
		BYTE *_b = new BYTE[n];
		memcpy(_b, d, n); 
		fprintf(stderr, "%% Read (%d): ", (int) n);
		for(int i = 0 ; i < n ; ++i) fprintf(stderr, "%02hhx ", _b[i]);
		delete[] _b;
		std::cerr << std::endl;
#endif
	}

#ifndef NBT_NO_GZ
	template<>
	void nbt_reader<gzip::igzstream>::do_read(char *d, size_t n) const
	{
		assert(d);

#	ifdef DEBUG
		char *_d = d;
		int _n = (int) n;
#	endif

		while(n--) is.get(*d++);

#	ifdef DEBUG
		BYTE *_b = new BYTE[_n];
		memcpy(_b, _d, _n); 
		fprintf(stderr, "%% Read (%d): ", _n);
		for(int i = 0 ; i < _n ; ++i) fprintf(stderr, "%02hhx ", _b[i]);
		delete[] _b;
		std::cerr << std::endl;
#	endif
	}
#endif
	
	template class nbt_reader<std::istream>;
#ifndef NBT_NO_GZ
	template class nbt_reader<gzip::igzstream>;
#endif
}

