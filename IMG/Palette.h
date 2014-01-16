#ifndef PALETTE_H
#define PALETTE_H

#include <iostream>
#include <assert.h>
#include <string.h>
#include "IMG.h"

namespace IMG
{
	class Palette
	{
		public:
			Palette(int = 16);
			Palette(const Palette&);
			~Palette();
			Palette& setColor(int, RGBA);
			Palette& setColor(RGBA *, int = 0, int = -1);
			RGBA get(int) const;
			RGBA& operator[](int);
			RGBA operator[](int) const;
		private:
			RGBA *palette;
			int size;
	};
}

#endif


