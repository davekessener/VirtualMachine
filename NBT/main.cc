#include <iostream>
#include "Terminal.h"
#include "Curse.h"
#include "Scene.h"
#include "Editor.h"

#include "Game.h"

int main(int argc, char *argv[])
{
	display::Terminal::create<ncurses::Curse>();

	Editor e;
	Game g;

	Scene::play(&g);

    return 0;
}

