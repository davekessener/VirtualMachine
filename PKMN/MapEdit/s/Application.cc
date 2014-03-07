#include "Application.h"

namespace surface
{
	void Application::handle(SDL_Event& e)
	{
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

