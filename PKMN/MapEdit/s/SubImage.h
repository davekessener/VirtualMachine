#ifndef _SUB_IMAGE_H
#define _SUB_IMAGE_H

#include "Image.h"

class SubImage : public Image
{
	public:
		SubImage(Image *, int, int, int, int);
		~SubImage( );
		void blit(const Image *, Point, Rect);
		void getUnderlying(SDL_Rect *) const;
		explicit operator SDL_Texture *( ) const;
	private:
		Image *_i;
		int _x, _y;
};

#endif

