#include "Application.h"

namespace surface
{
	void Application::handle(SDL_Event& e)
	{
		static Surface *locked = NULL;

		switch(e.type)
		{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				locked = lock(e.button.x, e.button.y);
				locked->mouseDown(SDL_BUTTON(e.button.button), e.button.x, e.button.y);
				break;
			case SDL_MOUSEMOTION:
				(locked ? locked : lock(e.motion.x, e.motion.y))->mouseDrag(e.motion.state, e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if(locked)
				{
					locked->mouseUp(SDL_BUTTON(e.button.button), e.button.x, e.button.y);
					locked = NULL;
				}
				break;
		}
	}
	
	void Application::execute(const std::string& cmd)
	{
		if(cmd.compare("quit") == 0) running = false;
	}
	
	Application::Application(Image *i) : Surface(i, 0, 0), running(true)
	{
		registerSurface(status = new StatusBar(i, "Application"));
		content = NULL;
	}
	
	Application::~Application(void)
	{
		delete status;
		delete content;
	}
}

