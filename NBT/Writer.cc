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
		QWORD qw = 0; memcpy(&qw, d, n); std::reverse(reinterpret_cast<BYTE *>(&qw), reinterpret_cast<BYTE *>(&qw) + n);
		fprintf(stderr, "%% Wrote (%d): 0x%0*lx\n", (int) n, 2 * (int) n, qw);
#endif
	}

	template<>
	void nbt_writer<gzip::ogzstream>::do_write(char *d, size_t n) const
	{
		assert(d);

#ifndef __WRITER_DEBUG
		QWORD qw = 0; memcpy(&qw, d, n); std::reverse(reinterpret_cast<BYTE *>(&qw), reinterpret_cast<BYTE *>(&qw) + n);
		fprintf(stderr, "%% Wrote (%d): 0x%0*lx\n", (int) n, 2 * (int) n, qw);
#endif

		while(n--) os << *d++;
	}

	template class nbt_writer<std::ostream>;
	template class nbt_writer<gzip::ogzstream>;
}

