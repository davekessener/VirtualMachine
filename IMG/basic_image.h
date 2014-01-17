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
			inline int Width( ) const { return width; }
			inline int Height( ) const { return height; }
			int Size( ) const;
		protected:
			void prepareBMH(bitmap_header_t *bmh, DIB_t *dib) const;
			void prepareDIB(DIB_t *dib) const;
			inline T *getImageData( ) const { return image; }
			static const size_t ELEM_SIZE = sizeof(T);
		private:
			unsigned width, height;
			T *image;

			friend class palette_image;
//			template<class C>
//			friend basic_image<C>::operator basic_image<T>();
	};

	typedef basic_image<RGBA> rgba_image;
#include "basic_image.cpp"
}

#endif

