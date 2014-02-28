#define SCREEN_MAIN
#include "Screen.h"
#undef SCREEN_MAIN

Screen::Screen(void) : window(NULL), renderer(NULL)
{
	loadKeyMap(std::string(""), keyMap);
}

Screen::~Screen(void)
{
	if(window) finalize();
}

void Screen::init(void)
{
	if(window) return;

	if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();

	if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == -1) throw SDLException(IMG_GetError());

	window = SDL_CreateWindow("Pokemon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(!window) throw SDLException();

	renderer = SDL_CreateRenderer(window, -1, 0);

	if(!renderer) throw SDLException();
}

void Screen::finalize(void)
{
	if(window)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		window = NULL;
		renderer = NULL;
	}

	quitFn = quitFn_t{};
	while(!keyPressed.empty()) keyPressed.pop_back();
}

void Screen::flush(void)
{
	if(!window) throw SDLException();
	SDL_RenderPresent(renderer);
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
	SDL_RenderClear(renderer);
}

void Screen::eraseLine(void)
{
}

int Screen::getChar(void)
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				quitFn();
				break;
			case SDL_KEYDOWN:
				{
					auto i = keyMap.find(e.key.keysym.sym);
					if(i != keyMap.end()) pressKey(i->second);
				}
				break;
			case SDL_KEYUP:
				{
					auto i = keyMap.find(e.key.keysym.sym);
					if(i != keyMap.end()) releaseKey(i->second);
				}
				break;
		}
	}

	return keyPressed.empty() ? Controls::NONE : keyPressed.back();
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

void Screen::onQuit(quitFn_t q)
{
	quitFn = q;
}

SDL_Texture *Screen::loadImage(const std::string& path)
{
	SDL_Texture *tex = IMG_LoadTexture(renderer, path.c_str());

	if(!tex) throw SDLException();

	return tex;
}

void Screen::toScreen(SDL_Texture *img, SDL_Rect o, SDL_Rect d)
{
	if(o.w > d.w || o.h > d.h) throw SDLException();

	SDL_RenderCopy(renderer, img, &o, &d);
}

void Screen::loadKeyMap(const std::string& path, std::map<int, int>& m)
{
	m.clear();

	m[SDLK_w] = Controls::UP;
	m[SDLK_a] = Controls::LEFT;
	m[SDLK_d] = Controls::RIGHT;
	m[SDLK_s] = Controls::DOWN;
	m[SDLK_l] = Controls::A;
	m[SDLK_p] = Controls::B;
	m[SDLK_k] = Controls::X;
	m[SDLK_o] = Controls::Y;
	m[SDLK_c] = Controls::L;
	m[SDLK_m] = Controls::R;
	m[SDLK_RETURN] = Controls::START;
	m[SDLK_BACKSPACE] = Controls::SELECT;
}

void Screen::pressKey(int k)
{
	for(auto i = keyPressed.begin() ; i != keyPressed.end() ; ++i)
	{
		if(*i == k) return;
	}

	keyPressed.push_back(k);
}

void Screen::releaseKey(int k)
{
	for(auto i = keyPressed.begin() ; i != keyPressed.end() ; ++i)
	{
		if(*i == k)
		{
			keyPressed.erase(i);
			return;
		}
	}

	throw SDLException("ERR: Key '%d' wasn't pressed yet released.", k);
}

