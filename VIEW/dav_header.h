#ifndef DAVHEADER_H
#define DAVHEADER_H

#include "inc.h"

namespace dav
{
	static const DWORD DAV_MAGIC(('D' << 24) | ('a' << 16) | ('V' << 8) | '!');
	
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
		DWORD imagecount;
	} __attribute__((packed));

	struct data_t
	{
		DWORD id;
		QWORD name;
		QWORD viewed_ms;
		DWORD viewed_c;
	} __attribute__((packed));
}

#endif

