#include <iostream>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDLException.h"

void pause(std::function<void(SDL_Renderer *)>render, SDL_Renderer *renderer, SDL_Window *window)
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

		SDL_RenderClear(renderer);
		render(renderer);
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

		SDL_Rect ro = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, rt(ro);

		SDL_Texture *img = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_Texture *ts  = IMG_LoadTexture(renderer, "../resource/tileset.png");

		SDL_SetTextureBlendMode(img, SDL_BLENDMODE_BLEND);

		ro.w = ro.h = rt.w = rt.h = 64;

		SDL_SetRenderTarget(renderer, img);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, NULL);

		SDL_SetRenderTarget(renderer, img);
		SDL_RenderCopy(renderer, ts, &ro, &rt);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, img, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, img);
		rt.x = 16; rt.y = 16; rt.w = rt.h = 16;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(renderer, &rt);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, img, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, img);
		rt = ro;
		ro.x = 96;
		SDL_RenderCopy(renderer, ts, &ro, &rt);
		SDL_SetRenderTarget(renderer, NULL);

		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

//		SDL_SetRenderTarget(renderer, img);
//		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//		SDL_RenderFillRect(renderer, &r);
//		SDL_SetRenderTarget(renderer, NULL);
//
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		pause([img](SDL_Renderer *_r) { SDL_RenderCopy(_r, img, NULL, NULL); }, renderer, window);

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

