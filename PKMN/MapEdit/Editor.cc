#include "Editor.h"

int Editor::run(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
	if(IMG_Init(IMG_INIT_PNG) == -1) throw SDLException(IMG_GetError());

	Editor e;
	AsyncIn in;

	try
	{
		Timer t;
		SDL_Event ev;
		std::string command;

		std::cout << "> " << std::flush;

		while(e.running)
		{
			if(SDL_PollEvent(&ev))
			{
				e.handle(ev);
			}

			if(in.pollCommand(command))
			{
				e.execute(command);
				if(e.running) std::cout << "> " << std::flush;
			}

			e.render();

			if(e.running) t.keepRate(FRAME_RATE);
		}
	}
	catch(const SDLException& ex)
	{
		e.save("./.bckup.map");
		std::cerr << "SDL-Err: " << ex.what() << std::endl;
		LOG("[FATAL] '%s'", ex.what());
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
	windows[tileset.getID()] = &tileset;

	cmds["quit"] = [this](cmd_params_t p) { running = false; };
}

Editor::~Editor(void)
{
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

void Editor::handle(SDL_Event& e)
{
	if(windows.count(e.button.windowID) > 0)
	{
		Window *w = windows.at(e.button.windowID);

		switch(e.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				w->onClick(Button::LEFT, e.button.x, e.button.y);
				break;
		}
	}
	else
	{
		switch(e.type)
		{
			case SDL_QUIT:
				running = false;
				break;
		}
	}
}

void Editor::render(void)
{
	for(auto i = windows.begin() ; i != windows.end() ; ++i)
	{
		i->second->refresh();
	}
}

