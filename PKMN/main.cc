#include <iostream>
#include "Game.h"
#include "Screen.h"

int main(void)
{
	display::Terminal::create<Screen>();

	Game g;

	Scene::play(&g);

	return 0;
}

