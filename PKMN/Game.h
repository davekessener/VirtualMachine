#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "SDLImage.h"

#ifdef GAME_MAIN
#include "Screen.h"
#endif

class Game : public Scene
{
	public:
		Game( );
		~Game( );
		void input(int);
		void update(int);
		void refresh( );
	protected:
	private:
		SDLImage tstimage;
};

#endif

