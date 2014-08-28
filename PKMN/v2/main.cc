#include <iostream>
#include <vector>
#include <unistd.h>
#include "Scene.h"

int main(int argc, char *argv[])
{
	chdir("resource/");

	return pkmn::Scene::run(std::vector<std::string>(argv, argv + argc));
}

