#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDLException.h"

void pause(SDL_Texture *img, SDL_Renderer *renderer, SDL_Window *window)
{
	bool running = true;
	SDL_Event e;

	SDL_Rect r = {10, 10, 200, 200};

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

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, img, &r, &r);
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}
}

int main(void)
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	try
	{
		SDL_Window *window;
		SDL_Renderer *renderer;

		if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
	
		if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == -1) throw SDLException(IMG_GetError());
	
		window = SDL_CreateWindow("Pokemon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
		if(!window) throw SDLException();
	
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

		if(!renderer) throw SDLException();

		SDL_Rect r;

		r.x = r.y = 0;
		r.w = SCREEN_WIDTH;
		r.h = SCREEN_HEIGHT;

		SDL_Texture *img = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

		r.x = r.y = 10;
		r.w = r.h = 200;

		SDL_SetRenderTarget(renderer, img);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &r);
		SDL_SetRenderTarget(renderer, NULL);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		pause(img, renderer, window);

		SDL_DestroyTexture(img);
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

