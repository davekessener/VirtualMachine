#ifndef SDLIMAGE_H
#define SDLIMAGE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <cassert>
#include "Screen.h"

#ifdef SDLIMAGE_MAIN
#endif

class SDLImage
{
	public:
		SDLImage( );
		SDLImage(const std::string&);
		SDLImage(int, int);
		~SDLImage( );
		void create(int, int);
		void open(const std::string&);
		void close( );
		void blit(const SDLImage&, SDL_Rect, SDL_Rect);
		void toScreen( );
		void startBlit( );
		void endBlit( );
		explicit operator SDL_Texture *( ) const;
	private:
		SDL_Texture *img;
};

#endif

