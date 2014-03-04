#ifndef MAP_TILESETWINDOW_H
#define MAP_TILESETWINDOW_H

#include <SDL_image.h>
#include "Window.h"
#include <Misc/Logger.h>

class TilesetWindow : public Window
{
	public:
		TilesetWindow( );
		~TilesetWindow( );
		void onClick(button_t, int, int);
	private:
		void draw( );
		SDL_Window *createWindow( );

		SDL_Texture *ts;

		static const int WIDTH = 1024;
		static const int HEIGHT = 1024;
};

#endif

