#ifndef MAP_TILESETSURFACE_H
#define MAP_TILESETSURFACE_H

#include "Surface.h"

class TilesetSurface : public Surface
{
	public:
		TilesetSurface(int, int, int, int);
		~TilesetSurface( );
		void setBaseColor(std::uint8_t, std::uint8_t, std::uint8_t);
		void showGrid(bool f) { if(f) showG |= SHOW_GRID; else showG &= ~SHOW_GRID; }
		void simpleBackground(bool f) { if(f) showG |= SIMPLE_BG; else showG &= ~SIMPLE_BG; }
		int selection( ) const { return select; }

		static const int SHOW_GRID = 1;
		static const int SIMPLE_BG = 2;
	protected:
		void draw(SDLImage&, int, int);
		void onClick(button_t, int, int);
		std::pair<int, int> getScrollSize( );
	private:
		int select, showG;
		SDLImage tileset, bg, grid;
		SDL_Color bgC;
};

#endif

