#include <iostream>
#include "Terminal.h"
#include "Curse.h"
#include "Scene.h"
#include "Editor.h"
#include "Logger.h"

#include "Game.h"

int main(int argc, char *argv[])
{
	display::Terminal::create<ncurses::Curse>();

	Editor e;

	Scene::play(&e);

    return 0;
}

