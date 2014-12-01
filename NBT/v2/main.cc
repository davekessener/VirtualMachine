#include <iostream>
#include <vector>
#include "Terminal.h"
#include "Curse.h"
#include "Scene.h"
#include "Editor.h"

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv + 1, argv + argc);
	display::Terminal::create<ncurses::Curse>();

	Editor e(args);

	Scene::play(&e);

    return 0;
}

