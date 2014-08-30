#include <iostream>
#include <vector>
#include <unistd.h>
#include "Scene.h"

int main(int argc, char *argv[])
try
{
	chdir("resource/");

	return pkmn::Scene::run(std::vector<std::string>(argv, argv + argc));
}
catch(const std::string& e)
{
	std::cerr << "Aborted with message:\n" << e << std::endl;
}

