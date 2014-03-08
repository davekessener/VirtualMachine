#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
		virtual int X( ) { return 0; }
		virtual int Y( ) { return 0; }
		int width( ) const { return _width; }
		int height( ) const { return _height; }
		void gradientFill(color_rgba_t, color_rgba_t, color_rgba_t, color_rgba_t);
		void fillRect(color_rgba_t, int, int, int, int);
		void drawLine(color_rgba_t, int, int , int , int);
		void renderText(const std::string&, int, int, color_rgba_t = 0x000000ff);
		virtual void getUnderlying(SDL_Rect *) const;
		virtual explicit operator SDL_Texture *( ) const;
		static void init(SDL_Renderer *r, TTF_Font *f) { render = r; font  = f; }
	protected:
		int _width, _height;
	private:
		Image(SDL_Surface *);
		SDL_Texture *img;
		static SDL_Renderer *render;
		static TTF_Font *font;
		static bool isBlitting;
};

#endif

