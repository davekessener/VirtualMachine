#define GAME_MAIN
#include "Game.h"
#undef GAME_MAIN

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::init(void)
{
	Screen::instance().onQuit(std::bind(&Game::quit, this));
	Controller::create("data.nbt");
	pushInputFunction(std::bind(&Controller::input, &Controller::instance(), std::placeholders::_1));
}

void Game::finalize(void)
{
	Controller::destroy();
}

void Game::input(int in)
{
	switch(in)
	{
		case Controls::A:
			break;
		case Controls::B:
			break;
		case Controls::X:
			break;
		case Controls::Y:
			break;
		case Controls::UP:
			break;
		case Controls::DOWN:
			break;
		case Controls::LEFT:
			break;
		case Controls::RIGHT:
			break;
		case Controls::START:
			break;
		case Controls::SELECT:
			break;
		case Controls::L:
			break;
		case Controls::R:
			break;
		default:
			break;
	}
}

void Game::update(void)
{
	Controller::instance().update();
}

void Game::refresh(void)
{
	Controller::instance().render();
}

