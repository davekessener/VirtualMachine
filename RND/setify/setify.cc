#include <iostream>
#include <fstream>
#include <vector>
#include <set>

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);
	std::set<std::string> buf;

	std::istream *in = &std::cin;
	std::ostream *out = &std::cout;

	for(auto i1 = args.cbegin(), i2 = args.cend() ; i1 != i2 ; ++i1)
	{
		if(*i1 == "-o")
		{
			if(++i1 == i2) throw std::string("ERR: No filename specified after '-o'!");

			out = new std::ofstream(*i1, std::ios::out);

			if(!*out) throw std::string("ERR: Couldn't open '") + *i1 + "' for write!";
		}
		else if(*i1 == "-i")
		{
			if(++i1 == i2) throw std::string("ERR: No filename specified after '-i'!");

			in = new std::ifstream(*i1, std::ios::in);

			if(!*in) throw std::string("ERR: Couldn't open '") + *i1 + "' for read!";
		}
	}

	while(*in && !in->eof())
	{
		std::string s("");

		std::getline(*in, s);

		if(!s.empty() && buf.insert(s).second)
		{
			*out << s << "\n";
		}
	}

	if(in != &std::cin) { static_cast<std::ifstream *>(in)->close(); delete in; }
	if(out != &std::cout) { static_cast<std::ofstream *>(out)->close(); delete out; }

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

