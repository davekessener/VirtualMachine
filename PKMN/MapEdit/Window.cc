#include "Window.h"

Window::Window(void) : win(NULL), render(NULL)
{
}

Window::~Window(void)
{
	if(render) SDL_DestroyRenderer(render);
	if(win) SDL_DestroyWindow(win);
}

void Window::init(void)
{
	if(!(win = createWindow())) throw SDLException();
	if(!(render = SDL_CreateRenderer(win, -1, SDL_RENDERER_TARGETTEXTURE))) throw SDLException();
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
}

void Window::setTitle(const std::string& s)
{
	title = s;

	std::ostringstream oss;

	oss << '#' << static_cast<int>(getID()) << " '" << s << "'";

	SDL_SetWindowTitle(win, oss.str().c_str());
}

void Window::refresh(void)
{
	draw();

	SDL_RenderPresent(render);
	SDL_UpdateWindowSurface(win);
}

