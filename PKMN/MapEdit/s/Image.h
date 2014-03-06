#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cassert>
#include "Geometry.h"

class Image
{
	typedef unsigned int color_rgba_t;

	public:
		Image( );
		Image(const std::string&);
		Image(int, int);
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
		void gradientFill(color_rgba_t, color_rgba_t, color_rgba_t, color_rgba_t);
		virtual void getUnderlying(SDL_Rect *) const;
		virtual explicit operator SDL_Texture *( ) const;
		static void init(SDL_Renderer *r) { render = r; }
	protected:
		int _width, _height;
	private:
		SDL_Texture *img;
		static SDL_Renderer *render;
		static bool isBlitting;
};

#endif

