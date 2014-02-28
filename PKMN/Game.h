#ifndef GAME_H
#define GAME_H

#include <functional>
#include "Scene.h"
#include "TileMap.h"
#include "Logger.h"

#ifdef GAME_MAIN
#include "Screen.h"
#endif

class Game : public Scene
{
	public:
		Game( );
		~Game( );
		void init( );
		void finalize( );
		void input(int);
		void update(int);
		void refresh( );
	protected:
	private:
		Sprite *sprite;
		TileMap *tileset;
		int x, y, dx, dy;
};

#endif

