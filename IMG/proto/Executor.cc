#include "Executor.h"

int Executor::run(int argc, char *argv[])
{
	SDL_Surface *image, *screen;

	SDL_Init(SDL_INIT_EVERYTHING);

	screen = SDL_SetVideoMode(128, 128, 32, SDL_SWSURFACE);
	image = SDL_LoadBMP("test.bmp");

	SDL_BlitSurface(image, NULL, screen, NULL);
	SDL_Flip(screen);

	SDL_Delay(2000);

	SDL_FreeSurface(image);

	SDL_Quit();

	return EXIT_SUCCESS;
}

