#ifndef DAVHEADER_H
#define DAVHEADER_H

static const DWORD DAV_MAGIC(('D' << 24) | ('a' << 16) | ('V' << 8) | '!');

struct dav_header_t
{
	DWORD id;
	WORD width;
	WORD height;
	DWORD filesize;
} __attribute__((packed));

#endif

