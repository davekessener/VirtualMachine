#ifndef GAME_H
#define GAME_H

#include <functional>
#include "Scene.h"
#include "Misc/Logger.h"
#include "Overworld/Controller.h"

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
		void update( );
		void refresh( );
	protected:
	private:
};

#endif

