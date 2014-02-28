#include <iostream>
#include "Game.h"
#include "Screen.h"
#include "SDLException.h"

int main(void)
{
	try
	{
		display::Terminal::create<Screen>();

		Game g;

		Scene::play(&g);
	}
	catch(const SDLException& e)
	{
		std::cerr << "SDL-Exception: " << e.what() << std::endl;

		return 1;
	}

	return 0;
}

