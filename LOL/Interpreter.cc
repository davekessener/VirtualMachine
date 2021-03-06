#include <iostream>
#include "Interpreter.h"
#include "IntImpl.h"
#include <dav/Logger.h>
#include <dav/utils.h>
#include <lex/lex.h>
#include <aux>

namespace lol {

int Interpreter::doRun(const params_t& args)
{
	using dav::utils::next;

	while(true)
	{
		std::string in(input());

		if(in == "quit") break;

		while(!in.empty())
		{
			writeLine(next(in));
		}
	}

	return 0;
}

std::string Interpreter::input(void) const
{
	write("> ");
	return readLine();
}

Interpreter& Interpreter::instance(void)
{
	static Interpreter i;
	return i;
}

Interpreter::Interpreter(void) : impl_(new Impl)
{
}

Interpreter::~Interpreter(void)
{
	delete impl_;
}

std::string Interpreter::readLine(void)
{
	std::string s;
	while(s.empty()) std::getline(std::cin, s);
	LOG("$$$ Read: '%s'", s.data());
	return s;
}

void Interpreter::write(const std::string& line)
{
	std::cout << line;
	LOG("$$$ Wrote: '%s'", line.data());
}

void Interpreter::writeLine(const std::string& line)
{
	write(line);
	std::cout << std::endl;
}

}

