#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>

#ifdef IMAGE_MAIN
#endif

namespace image
{
	struct Rect
	{
		int x, y, w, h;
		Rect(int _x = -1, int _y = -1, int _w = -1, int _h = -1) : x(_x), y(_y), w(_w), h(_h) { }
	};

	class Image
	{
		public:
			virtual ~Image( );
			virtual void open(const std::string&) = 0;
			virtual void close( ) = 0;
			virtual void blit(const Image&, Rect = Rect(), Rect = Rect()) = 0;
		private:
	};
}

#endif

