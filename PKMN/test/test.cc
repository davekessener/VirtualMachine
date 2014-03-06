#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDLException.h"

void pause( );

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
	
		window = SDL_CreateWindow("Pokemon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
		if(!window) throw SDLException();
	
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

		if(!renderer) throw SDLException();

		SDL_Rect r;

		r.x = r.y = 0;
		r.w = SCREEN_WIDTH;
		r.h = SCREEN_HEIGHT;

		SDL_Texture *img = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

		unsigned int *pix = new unsigned int[SCREEN_WIDTH * SCREEN_HEIGHT];
		unsigned int c = 0x0000ffff;

		for(int i = 0 ; i < SCREEN_WIDTH * SCREEN_HEIGHT ; ++i)
		{
			pix[i] = c;
		}

		SDL_UpdateTexture(img, NULL, pix, SCREEN_WIDTH * sizeof(unsigned int));

		delete pix;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, img, &r, &r);
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);

		pause();

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

