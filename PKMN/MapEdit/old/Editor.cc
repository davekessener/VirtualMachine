#include "Editor.h"

int Editor::run(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
	if(IMG_Init(IMG_INIT_PNG) == -1) throw SDLException(IMG_GetError());

	Editor e;

	try
	{
		while(std::cin && e.running)
		{
			std::string cmd;

			std::cout << "> ";
			std::getline(std::cin, cmd);

			if(!cmd.empty()) e.execute(cmd);
		}
	}
	catch(const SDLException& ex)
	{
		e.save("./.bckup.map");
		std::cerr << "SDL-Err: " << ex.what() << std::endl;
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}

// # ---------------------------------------------------------------------------

Editor::cmd_params_t Editor::parse(const std::string& c)
{
	std::vector<std::string> v;
	std::istringstream iss(c);

	while(iss)
	{
		std::string s;
		iss >> s;
		if(!s.empty()) v.push_back(s);
	}

	return v;
}

// # ---------------------------------------------------------------------------

Editor::Editor(void) : running(true)
{
	cmds["quit"] = [this](cmd_params_t p) { running = false; };
}

Editor::~Editor(void)
{
	tsWin.join();
}

void Editor::execute(const std::string& cmd)
{
	cmd_params_t p = parse(cmd);

	if(p.size() < 1) return;

	std::string c = p.at(0);

	if(cmds.count(c) > 0)
	{
		cmds.at(c)(p);
	}
}

void Editor::save(const std::string& fn)
{
}

