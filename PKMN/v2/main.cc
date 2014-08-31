#include <iostream>
#include <vector>
#include <unistd.h>
#include "Scene.h"
#include "Editor/Editor.h"

int main(int argc, char *argv[])
try
{
	chdir("resource/");

	params_t args(argv, argv + argc);

	if(args.size() > 1 && (args.at(1) == "--editor" || args.at(1) == "-e"))
	{
		return editor::Editor::run(args);
	}
	else
	{
		return pkmn::Scene::run(args);
	}
}
catch(const std::string& e)
{
	std::cerr << "Aborted with message:\n" << e << std::endl;
}

