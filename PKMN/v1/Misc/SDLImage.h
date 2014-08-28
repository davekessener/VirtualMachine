#ifndef SDLIMAGE_H
#define SDLIMAGE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDLException.h"
#include <cassert>

#ifdef SDLIMAGE_MAIN
#endif

class SDLImage
{
	public:
		SDLImage(SDL_Renderer * = NULL);
		SDLImage(SDL_Renderer *, const std::string&);
		SDLImage(SDL_Renderer *,int, int);
		~SDLImage( );
		void setRenderer(SDL_Renderer *r) { render = r; }
		void create(int, int);
		void open(const std::string&);
		void close( );
		void blit(const SDLImage&, SDL_Rect, SDL_Rect);
		void startBlit( );
		void endBlit( );
		void erase( );
		bool opened( ) { return img != NULL; }
		int width( ) const { return _width; }
		int height( ) const { return _height; }
		explicit operator SDL_Texture *( ) const;
	private:
		int _width, _height;
		SDL_Renderer *render;
		SDL_Texture *img;
		static bool isBlitting;
};

#endif

