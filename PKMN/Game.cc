#define GAME_MAIN
#include "Game.h"
#undef GAME_MAIN

Game::Game(void) : tstimage("./test.bmp")
{
}

Game::~Game(void)
{
}

void Game::input(int in)
{
	if(Screen::instance().shouldQuit()) quit();
}

void Game::update(int ms)
{
}

void Game::refresh(void)
{
	tstimage.toScreen();
}

