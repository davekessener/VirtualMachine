#include <iostream>
#include "Terminal.h"
#include "Curse.h"
#include "Scene.h"
#include "Editor.h"

int main(int argc, char *argv[])
{
	display::Terminal::create<ncurses::Curse>();

	Editor e;

	Scene::play(&e);

    return 0;
}

