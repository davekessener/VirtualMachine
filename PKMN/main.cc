#include <NBT/NBT.h>
#include <Misc/SDLException.h>
#include <iostream>
#include <vector>
#include "Game.h"
#include "Screen.h"
#include <Editor/Editor.h>

int launchGame( );
int launchEditor( );

int main(int argc, char *argv[])
{
	chdir("resource");

	if(argc > 1 && std::string(argv[1]).compare("--editor") == 0)
	{
		return launchEditor();
	}
	else
	{
		return launchGame();
	}
}

int launchEditor(void)
{
	return editor::Editor::run(0, NULL);
}

int launchGame(void)
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

