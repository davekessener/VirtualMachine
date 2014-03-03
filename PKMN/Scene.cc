#define SCENE_MAIN
#include "Scene.h"
#undef SCENE_MAIN

using display::Terminal;

Scene *Scene::scene = NULL;

void Scene::play(Scene *s)
{
	if(scene == NULL)
	{
		scene = s;
		Terminal::instance().init();
		scene->run();
		Terminal::instance().finalize();
		scene = NULL;
	}
	else
	{
		scene = s;
	}
}

void Scene::run(void)
{
	running = true;

	init();

	timer.reset();
	while(running)
	{
		if(scene != this)
		{
			suspend();
			scene->run();
			scene = const_cast<Scene *>(this);
			wake();
		}

		int in = Terminal::instance().getChar();

		if(in != display::Keys::ERR)
		{
			if(!inputFns.empty())
			{
				if(inputFns.top()(in))
				{
					inputFns.pop();
				}
			}
			else
			{
				input(in);
			}
		}

		update();

		Terminal::instance().eraseScreen();

		refresh();

		Terminal::instance().flush();

		timer.keepRate(FRAME_RATE);
	}

	finalize();
}

void Scene::init(void)
{
}

void Scene::suspend(void)
{
}

void Scene::wake(void)
{
}

void Scene::finalize(void)
{
}

void Scene::quit(void)
{
	running = false;
}

void Scene::pushInputFunction(inputFn_t inputFn)
{
	inputFns.push(inputFn);
}

