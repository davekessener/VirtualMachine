#define EDITOR_MAIN
#include "Editor.h"
#undef EDITOR_MAIN

Editor::Editor(void)
{
}

Editor::~Editor(void)
{
}

void Editor::registerCommands(void)
{
	registerCommand("quit", std::bind(&Editor::quit, this));
}

void Editor::init(void)
{
	CommandableScene::init();
}

void Editor::suspend(void)
{
}

void Editor::wake(void)
{
}

void Editor::input(int in)
{
	if(in == 'q') quit();
}

void Editor::update(int ms)
{
}

void Editor::refresh(void)
{
	CommandableScene::refresh();
}

void Editor::finalize(void)
{
}


