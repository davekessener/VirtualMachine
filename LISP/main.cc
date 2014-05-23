#include <iostream>
#include <vector>
#include "lisp.h"

int main(int argc, char *argv[])
{
	return lisp::Lisp().run(std::vector<std::string>(argv, argv + argc));
}

