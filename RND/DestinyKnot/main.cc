#include <iostream>
#include <vector>

int main(int argc, char * argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	if(args.size() != 3) throw std::string("ERR: ./dk p1 p2 g // 11x111");
	if(args.at(0).size() != args.at(1).size() || args.at(0).size() != args.at(2).size() ||
		args.at(0).size() != 6) throw std::string("ERR: 11x111");

	const char *p1 = args.at(0).c_str(), *p2 = args.at(1).c_str(), *ch = args.at(2).c_str();
	double v = 0.0;

	for(int iv = 0 ; iv < 6 ; ++iv)
	{
		double t = 1.0;

		for(int i = 0 ; i < 6 ; ++i)
		{
			if(ch[i] == 'x') continue;

			double tt = 0.0;

			if(i == iv)
			{
				tt = 0.03125;
			}
			else
			{
				if(p1[i] != 'x') tt += 0.5;
				if(p2[i] != 'x') tt += 0.5;
			}

			if((t *= tt) == 0.0) break;
		}

		v += t;
	}
	
	v /= 6;

	std::cout << (v * 100) << "% / 1 in " << ((int)((1.0 / v) * 100) / 100.0) << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

