#include <iostream>
#include <vector>
#include "Interpreter.h"

int main(int argc, char *argv[])
{
	return lol::Interpreter::instance().run(std::vector<std::string>(argv, argv + argc));
}

