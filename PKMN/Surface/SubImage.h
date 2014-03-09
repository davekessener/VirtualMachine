#ifndef _SUB_IMAGE_H
#define _SUB_IMAGE_H

#include "Image.h"

class SubImage : public Image
{
	public:
		SubImage(Image *, int, int, int, int);
		~SubImage( );
		void blit(const Image *, Point, Rect);
		int X( ) const { return _x + _i->X(); }
		int Y( ) const { return _y + _i->Y(); }
		explicit operator SDL_Texture *( ) const;
	private:
		Image *_i;
		int _x, _y;
};

#endif

