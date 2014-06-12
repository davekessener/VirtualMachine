#include "Run.h"

namespace siu
{
	Run::Run(void)
	{
	}

	int Run::run(int argc, char **argv)
	{
		std::vector<std::string> args(argv, argv + argc);

		init();

		preprocess(args);

		int r = doRun();

		cleanup();

		return r;
	}

	void Run::init(void)
	{
	}

	void Run::preprocess(const std::vector<std::string>& args)
	{
	}

	int Run::doRun(void)
	{
		return 0;
	}

	void Run::cleanup(void)
	{
	}
}

