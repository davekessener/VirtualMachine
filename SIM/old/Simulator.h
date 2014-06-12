#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

class Simulator
{
	public:
		Simulator();
		~Simulator();
		static int simulate(const char *);
	private:
		void scan(std::ifstream&);
		void parse(const std::string&);
		bool include_file(const char *);
		void process_chip(const char *);
		std::map<std::string, Chip> chips;
};

#endif

