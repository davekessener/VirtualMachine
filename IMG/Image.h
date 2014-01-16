#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "IMG.h"
#include "Palette.h"

namespace IMG
{
	class Image
	{
		public:
			struct LZ77
			{
				inline LZ77(void *d, int s) : d(d), s(s) {}
				void *d;
				int s;
			};

			Image(int, int);
			Image(const LZ77&);
			Image(const Image&);
			~Image();
			void toFile(FILE *);
			Image& setPalette(const Palette&);
			Image& setRect(void *, int, int, int, int);
			Image& hMirror();
			Image& vMirror();
			Image& offsetBy(int);
			Image& bitBlt(int, int, int, int, const Image&, int, int, int = -1);
			void clear();
			int getWidth() const;
			int getHeight() const;
			int getSize() const;
		private:
			uint32_t width, height;
			uint8_t *image;
			const Palette *palette;
	};
}

#endif

