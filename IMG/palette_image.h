#ifndef PALETTE_IMAGE_H
#define PALETTE_IMAGE_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "IMG.h"
#include "basic_image.h"
#include "Palette.h"

namespace IMG
{
	class palette_image : public basic_image<uint8_t>
	{
		public:
			struct LZ77
			{
				inline LZ77(void *d, int s) : d(d), s(s) {}
				void *d;
				int s;
			};

			palette_image(int width, int height, int bpp = 4);
			palette_image(const LZ77& lz77);
			palette_image(const palette_image&);
			void toFile(FILE *) const;
			palette_image& setPalette(const Palette& palette);
			operator rgba_image( ) const;
		private:
			Palette palette;
			int bpp;
	};
}

#endif

