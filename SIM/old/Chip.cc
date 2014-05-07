#include "Chip.h"

Chip::Chip(const std::string& n, int in, int out) : name(n)
{
	inp = in;
	outp = out;
}

Chip::Chip(const Chip& c) : name(c.name)
{
	inp = c.inp;
	outp = c.outp;
}

Chip::~Chip(void)
{
}

