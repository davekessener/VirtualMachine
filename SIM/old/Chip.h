#ifndef CHIP_H
#define CHIP_H

#include <iostream>

class Chip
{
	public:
		Chip(const std::string&, int, int);
		Chip(const Chip&);
		~Chip();
	private:
		std::string name;
		int inp, outp;
};

#endif

