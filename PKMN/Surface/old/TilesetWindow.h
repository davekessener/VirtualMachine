#ifndef TILESETWINDOW_H
#define TILESETWINDOW_H

#include <Misc/Logger.h>
#include "VEdit.h"

class TilesetWindow : public VEdit
{
	public:
		TilesetWindow( );
		~TilesetWindow( );
		std::uint32_t selected( ) { return select; }
		static const std::uint32_t INVALID = 0xffffffff;
	private:
		void onClick(button_t, int, int);
		void redraw( );
		SDL_Window *createWindow( );

		std::uint32_t select;
		SDL_Texture *ts;
		static const int SCREEN_WIDTH = 1024;
		static const int SCREEN_HEIGHT = 1024;
};

#endif

