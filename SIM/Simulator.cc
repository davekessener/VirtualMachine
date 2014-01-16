#include "Simulator.h"

Simulator::Simulator(void)
{
}

Simulator::~Simulator(void)
{
}

int Simulator::simulate(const char *fn)
{
	Simulator s;
	std::cout << "Simulating file '" << fn << "'." << std::endl;

	std::ifstream f(fn);

	if(f.is_open())
	{
		s.scan(f);
		f.close();

		return 0;
	}
	else
	{
		std::cout << "ERR: Couldn't open file '" << fn << "'!" << std::endl;
		return 1;
	}
}

void Simulator::parse(const std::string& s)
{
	std::cout << "Parsing line '" << s << "'" << std::endl;

	if(s[0] == '.')
	{
		if(s.length() >= 4 && !s.compare(0, 4, ".INC"))
		{
			const char *fn = s.c_str() + 4;
			while(*++fn == ' ');
			
			if(!include_file(fn))
			{
				std::cerr << "ERR: Couldn't open file '" << fn << "'!" << std::endl;
			}
		}
		else if(s.length() >= 5 && !s.compare(0, 5, ".CHIP"))
		{
			const char *chip = s.c_str() + 5;
			while(*++chip == ' ');
			process_chip(chip);
		}
		else
		{
			std::cerr << "ERR: Invalid preprocessor directive '" << s << "'!" << std::endl;
		}
	}
}

void Simulator::scan(std::ifstream& f)
{
	std::cout << "Scanning ..." << std::endl;

	std::string line;

	while(getline(f, line))
	{
		if(!line.empty())
		{
			parse(line);
		}
	}
}

bool Simulator::include_file(const char *fn)
{
	std::cout << "File '" << fn << "' included." << std::endl;

	std::ifstream f(fn);

	if(f.is_open())
	{
		scan(f);
		f.close();

		return true;
	}
	else
	{
		return false;
	}
}

void Simulator::process_chip(const char *line)
{
	std::cout << "Chip parsed." << std::endl;
}

