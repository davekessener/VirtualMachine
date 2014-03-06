#include "Editor.h"

int Editor::run(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
	if(IMG_Init(IMG_INIT_PNG) == -1) throw SDLException(IMG_GetError());

	SDL_DisplayMode dMode;

	if(SDL_GetCurrentDisplayMode(0, &dMode)) throw SDLException();

	{
		Editor e(dMode.w - 16, dMode.h - 64);
		AsyncIn in;

		try
		{
			Timer t;
			SDL_Event ev;
			std::string command;

			std::cout << "> " << std::flush;

			while(e.running)
			{
				while(SDL_PollEvent(&ev))
				{
					e.handle(ev);
				}

				while(in.pollCommand(command))
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
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}

// # ---------------------------------------------------------------------------

Editor::Editor(int w, int h) : running(true), width(w), height(h), inputSurface(NULL)
{
	cmds["quit"] = [this](cmd_params_t p) { running = false; };

	if(!(win = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 
			SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED))) throw SDLException();

	if(!(renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_TARGETTEXTURE))) throw SDLException();

	if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) throw SDLException();

	LOG("Starting editor with resulation of %d x %d", width, height);

	Surface::init(renderer);

	tileset = new TilesetSurface(3 * width / 4, 0, width - (3 * width / 4), height);
}

Editor::~Editor(void)
{
	delete tileset;

	Surface::freeResources();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
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

button_t Editor::toButton(std::int8_t k)
{
	button_t b = 0;

	if(k & SDL_BUTTON_LMASK)
	{
		b |= Key::LEFT;
	}

	if(k & SDL_BUTTON_MMASK)
	{
		b |= Key::MIDDLE;
	}

	if(k & SDL_BUTTON_RMASK)
	{
		b |= Key::RIGHT;
	}

	return b;
}

void Editor::onMouseDown(SDL_MouseButtonEvent e)
{
	if(button_t b = toButton(SDL_BUTTON(e.button)))
	{
		if(tileset->lock(e.x, e.y))
		{
			inputSurface = tileset;
			tileset->click(b, e.x, e.y);
		}
	}
}

void Editor::onMouseUp(SDL_MouseButtonEvent e)
{
	if(inputSurface)
	{
		inputSurface->unlock();
		inputSurface = NULL;
	}
}

void Editor::onMouseDrag(SDL_MouseMotionEvent e)
{
	if(e.state && inputSurface)
	{
		if(button_t b = toButton(e.state))
		{
			inputSurface->click(b, e.x, e.y);
		}
	}
}


void Editor::handle(SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseUp(e.button);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseDown(e.button);
			break;
		case SDL_MOUSEMOTION:
			onMouseDrag(e.motion);
			break;
	}
}

void Editor::render(void)
{
	tileset->render();

	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(win);
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

