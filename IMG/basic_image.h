#ifndef BASIC_IMAGE_H
#define BASIC_IMAGE_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "IMG.h"

namespace IMG
{
	template<class T>
	class basic_image
	{
		public:
			basic_image(int width = 0, int height = 0);
			basic_image(const basic_image<T>&);
			~basic_image( );
			virtual void toFile(std::fstream&) const;
			basic_image<T>& bitBlt(int x, int y, int dx, int dy, const basic_image<T>& src, int sx, int sy, T transcolor = 0, bool transparent = false);
			basic_image<T>& trunc(int x, int y, int dx = -1, int dy = -1);
			basic_image<T>& clear( );
			inline int Width( ) const { return width; }
			inline int Height( ) const { return height; }
			int Size( ) const;
		protected:
			void prepareBMH(bitmap_header_t *bmh, DIB_t *dib) const;
			void prepareDIB(DIB_t *dib) const;
			void write(std::fstream&) const;
			unsigned width, height;
			T *image;

			friend class palette_image;
			template<class>
			friend class flow_image;
	};

	typedef basic_image<RGBA> rgba_image;
#include "basic_image.cpp"
}

#endif

