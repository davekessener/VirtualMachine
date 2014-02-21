#include <iostream>
#include <vector>

int main(void)
{
	std::string p = "/this/is/a/path";
	std::vector<std::string> v;

	std::reverse(p.begin(), p.end());

	for(auto i = p.begin(), j = i ; i != p.end() ; ++i)
	{
		if(*i != '/') continue;
		if(i == j) { ++j; continue; }

		v.push_back(std::string(j, i));

		j = ++i;
	}

	v.push_back(std::string(j, i));

	for(std::string s : v)
	{
		std::cout << '\'' << s << '\'' << std::endl;
	}

	return 0;
}

