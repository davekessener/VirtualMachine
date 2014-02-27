#define SCREEN_MAIN
#include "Screen.h"
#undef SCREEN_MAIN

Screen::Screen(void) : screen(NULL)
{
	init();
}

Screen::~Screen(void)
{
	if(window) finalize();
}

void Screen::init(void)
{
	if(window) return;

	assert(!SDL_Init(SDL_INIT_VIDEO));
	assert(window = SDL_CreateWindow("Pokemon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
	screen = SDL_GetWindowSurface(window);

	if(!screen)
	{
		std::cerr << "SDL ERR: " << SDL_GetError() << std::endl;
		exit(1);
	}
	else
	{
		std::cerr << "ADDR: " << screen << std::endl;
		std::cerr.flush();
	}
}

void Screen::finalize(void)
{
	if(window)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		screen = NULL;
	}
}

void Screen::flush(void)
{
	SDL_UpdateWindowSurface(window);
}

void Screen::getCursorPos(int& x, int& y)
{
	x = y = -1;
}

void Screen::setCursorPos(int x, int y)
{
}

void Screen::getScreenSize(int& w, int& h)
{
	w = SCREEN_WIDTH;
	h = SCREEN_HEIGHT;
}

void Screen::showCursor(bool f)
{
}

void Screen::eraseScreen(void)
{
}

void Screen::eraseLine(void)
{
}

int Screen::getChar(void)
{
	return 0;
}

void Screen::pause(void)
{
}

void Screen::printf(const char *s, va_list l)
{
}

void Screen::printfw(const wchar_t *s, va_list l)
{
}

Screen& Screen::instance(void)
{
	return *static_cast<Screen *>(&display::Terminal::instance());
}

bool Screen::shouldQuit(void)
{
	SDL_Event e;

	if(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
		{
			return true;
		}
	}

	return false;
}

SDL_Surface *Screen::loadOptimizedImage(const std::string& path)
{
	SDL_Surface *img, *opt;

	if(!(img = SDL_LoadBMP(path.c_str())))
	{
		std::cerr << "SDL_ERR: " << SDL_GetError() << std::endl;
		exit(1);
	}

	if(!(opt = SDL_ConvertSurface(img, screen->format, 0)))
	{
		std::cerr << "SDL_ERR: " << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_FreeSurface(img);

	return opt;
}

void Screen::toScreen(SDL_Surface *img, SDL_Rect o, SDL_Rect d)
{
	assert(!(o.w > d.w || o.h > d.h));

	if(o.w < d.w || o.h < d.h)
	{
		SDL_BlitScaled(img, &o, screen, &d);
	}
	else
	{
		SDL_BlitSurface(img, &o, screen, &d);
	}
}

