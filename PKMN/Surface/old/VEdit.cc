#include "VEdit.h"

std::mutex VEdit::c_mutex;

VEdit::VEdit(const std::string& _t) : title(_t), thread(NULL), win(NULL), render(NULL), running(false)
{
}

VEdit::~VEdit(void)
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
}

void VEdit::start(void)
{
	if(thread) throw SDLException("ERR: Thread %s already running!", title.c_str());

	running = true;
	thread = new std::thread(&VEdit::run, this);
}

void VEdit::join(void)
{
	std::lock_guard<std::mutex> lock(c_mutex);

	if(!thread) return;
	
	running = false;
	thread->join();
	delete thread;
	thread = NULL;
}

void VEdit::init(void)
{
	if(!(win = createWindow())) throw SDLException();
	if(!(render = SDL_CreateRenderer(win, -1, SDL_RENDERER_TARGETTEXTURE))) throw SDLException();
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
	winID = SDL_GetWindowID(win);
}

void VEdit::run(void)
{
	Timer t;
	SDL_Event e;

	while(running)
	{
		if(RenderManager::poll(winID, &e))
		{
			switch(e.type)
			{
				case SDL_WINDOWEVENT:
					if(e.window.event == SDL_WINDOWEVENT_CLOSE) running = false;
					break;
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		redraw();
		
		SDL_RenderPresent(render);
		SDL_UpdateWindowSurface(win);

		t.keepRate(FRAME_RATE);
	}
}

