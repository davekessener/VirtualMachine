#include <iostream>
#include <sstream>
#include "stringtools.h"

int main(void)
{
	std::string s("	ld r0, [r1], $$-(:lbl*4||7) ;; hello, world! \"This is	a \\tstring.\" lol+:lbl -$ \t");
	int wc = 0;

	try
	{
		while(!s.empty())
		{
			std::string t(token_extract(s, wc));
			if(t.empty()) continue;
			std::cout << wc << ": '" << t << "'" << std::endl;
			wc += t.size();
		}
	}
	catch(const char *msg)
	{
		std::cerr << msg << std::endl;
	}

	return 0;
}

