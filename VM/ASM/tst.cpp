#include <iostream>
#include <fstream>

std::string& trim(std::string&);

int main(void)
{
	std::string line;
	std::ifstream in("test.txt");

	while(!in.eof())
	{
		std::getline(in, line);
		if(!trim(line).empty()) std::cout << line << std::endl;
	}

	return 0;
}

std::string& trim(std::string& s)
{
	std::string r, t;
	bool beg = false;

	for(auto i = s.cbegin() ; i != s.cend() ; ++i)
	{
		if(*i != ' ' && *i != '\0' && *i != '\t' && *i != '\n')
		{
			r += t;
			r.push_back(*i);
			t.clear();

			beg = true;
		}
		else if(beg)
		{
			t.push_back(*i);
		}
	}

	return s = r;
}

