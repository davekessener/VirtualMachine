#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <cassert>
#include "Geometry.h"

class Image
{
	public:
		Image(SDL_Renderer * = NULL);
		Image(SDL_Renderer *, const std::string&);
		Image(SDL_Renderer *, int, int);
		virtual ~Image( );
		void setRenderer(SDL_Renderer *r) { render = r; }
		void create(int, int);
		void open(const std::string&);
		void close( );
		virtual void blit(const Image *, Point, Rect);
		void startBlit( );
		void endBlit( );
		bool opened( ) { return img != NULL; }
		int width( ) const { return _width; }
		int height( ) const { return _height; }
		virtual void getUnderlying(SDL_Rect *) const;
		virtual explicit operator SDL_Texture *( ) const;
	protected:
		int _width, _height;
	private:
		SDL_Renderer *render;
		SDL_Texture *img;
		static bool isBlitting;
};

#endif

