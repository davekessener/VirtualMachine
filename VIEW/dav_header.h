#ifndef DAVHEADER_H
#define DAVHEADER_H

#include "inc.h"

namespace dav
{
	static const DWORD DAV_MAGIC('D' | ('a' << 8) | ('V' << 16) | ('!' << 24));
	
	struct imgheader_t
	{
		DWORD id;
		WORD width;
		WORD height;
		DWORD filesize;
	} __attribute__((packed));
	
	struct dataheader_t
	{
		DWORD id;
		QWORD hash;
		DWORD imgcount;
	} __attribute__((packed));

	struct data_t
	{
		QWORD name;
		QWORD viewed_ms;
		DWORD viewed_c;
	} __attribute__((packed));

	inline size_t hash(const std::string& s) { return std::hash<std::string>()(s); }
}

#endif

