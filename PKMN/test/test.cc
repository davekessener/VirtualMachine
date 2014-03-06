#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDLException.h"

class A
{
	public:
		A(int v) : _v(v) { std::cout << "A cons: " << _v << std::endl; }
		A(const A& a) : _v(a._v) { std::cout << "A c-cons: " << _v << std::endl; }
		~A( ) { std::cout << "A destr: " << _v << std::endl; }
		int get( ) const { return _v; }
	private:
		int _v;
};

class B : public A
{
	public:
		B( ) : A(member()) { std::cout << "B constr: " << p << std::endl; }
	private:
		static int *p;
		int member( );
};

int *B::p = NULL;

int B::member(void)
{
	p = new int;

	std::cout << "P is " << p << std::endl;

	int t = *p;
	t ^= reinterpret_cast<unsigned long>(p);

	return t;
}

void testfn(A&& a)
{
	std::cout << "A value: " << a.get() << std::endl;
}

void testfn(const A& a)
{
	std::cout << "const A& value: " << a.get() << std::endl;
}

void pause( );

int main(void)
{
	B a;
	testfn(A(42));
	testfn(a);
	return 0;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	try
	{
		SDL_Window *window;
		SDL_Renderer *renderer;

		if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
	
		if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == -1) throw SDLException(IMG_GetError());
	
		window = SDL_CreateWindow("Pokemon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
		if(!window) throw SDLException();
	
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

		if(!renderer) throw SDLException();

		SDL_Texture *tex = IMG_LoadTexture(renderer, "./test.png");

		if(!tex) throw SDLException();

		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);

		SDL_Rect ro, rt;

		ro.x = ro.y = rt.x = rt.y = 0;
		ro.w = w;
		ro.h = h;
		rt.w = SCREEN_WIDTH;
		rt.h = SCREEN_HEIGHT;

#define min(i,j) ((i)<(j)?(i):(j))
		rt.w = ro.w = min(ro.w, rt.w);
		rt.h = ro.h = min(ro.h, rt.h);
#undef min

		SDL_Texture *img = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetRenderTarget(renderer, img);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, tex, NULL, NULL);
		SDL_SetRenderTarget(renderer, NULL);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, img, &ro, &rt);
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);

		pause();

		SDL_DestroyTexture(tex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();

		return 0;
	}
	catch(const SDLException& e)
	{
		std::cerr << "SDL Err: " << e.what() << std::endl;
	}

	return 1;
}

void pause(void)
{
	bool running = true;
	SDL_Event e;

	while(running)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
			{
				running = false;
				break;
			}
		}
	}
}

