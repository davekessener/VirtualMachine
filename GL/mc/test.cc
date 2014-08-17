#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dav/bmp_header.h>

#define MXT_SIZE 1024

typedef uint8_t  BYTE;
typedef uint32_t RGBA;

void test(void)
{
	using namespace dav::img;

	bmp_header_t bmp;
	dib_header_t dib;
	BYTE *buf = (BYTE *) malloc(MXT_SIZE * MXT_SIZE * sizeof(RGBA));

	dib.headersize = sizeof(dib);
	dib.width = dib.height = MXT_SIZE;
	dib.planes = 1;
	dib.bpp = sizeof(RGBA) << 3;
	dib.compression = 0;
	dib.size = 0;
	dib.resolution.dx = dib.resolution.dy = BMP_RES;
	dib.palettecount = 0;
	dib.reserved = 0;

	bmp.id = BMP_MAGIC;
	bmp.reserved.reserved1 = bmp.reserved.reserved2 = 0;
	bmp.offset = sizeof(bmp) + sizeof(dib);
	bmp.filesize = bmp.offset + dib.width * dib.height * sizeof(RGBA);

	for(int i = 0 ; i < dib.width * dib.height * sizeof(RGBA) ; ++i)
	{
		buf[i] = rand() % 256;
	}

	FILE *f = fopen("test.bmp", "w");
	fwrite(&bmp, sizeof(bmp), 1, f);
	fwrite(&dib, sizeof(dib), 1, f);
	fwrite(buf, sizeof(RGBA), dib.width * dib.height, f);
	fclose(f);
}

