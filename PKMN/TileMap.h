#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include <initializer_list>
#include "SDLImage.h"
#include "Screen.h"

#ifdef TILEMAP_MAIN
#endif

class Sprite
{
	public:
		Sprite( );
		~Sprite( );
		void walk(int);
		void update( );
		int getX( ) const;
		int getY( ) const;
		int getDX( ) const;
		int getDY( ) const;
		void render( ) const;
	private:
		SDLImage sprite;
		int x, y, dx, dy;
		int facing;
};

class TileMap
{
	public:
		TileMap(int, int, std::initializer_list<int>);
		~TileMap( );
		void render(const Sprite&);
		void renderAt(int, int, int , int);
	private:
		int width, height;
		int *map;
		SDLImage tileset;
};

#endif

