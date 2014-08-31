#include "Scene.h"
#include "SceneMain.h"
#include "Config.h"
#include <dav/gl.h>

pkmn::Scene_ptr pkmn::Scene::scene_, pkmn::Scene::temp_;
int pkmn::Scene::width_, pkmn::Scene::height_;

namespace pkmn
{
	int Scene::run(const params_t& args)
	{
		using namespace dav;

		temp_ = Scene_ptr(new SceneMain(args));

		width_ = Config::SCREEN_WIDTH;
		height_ = Config::SCREEN_HEIGHT;
		int scale = Config::SCREEN_SCALE;

		sdl::set_init(&Scene::doInit);
		sdl::set_update(&Scene::doUpdate);
		sdl::set_input(&Scene::doKeyboard);

		sdl::start("Pokemon", width_ * scale, height_ * scale);

		return 0;
	}

	void Scene::doInit(int w, int h)
	{
		using namespace dav;

		gl::init2d(width_, height_);
	}

	bool Scene::doUpdate(int d)
	{
		using namespace dav;

		if(temp_)
		{
			if(scene_)
			{
				if(scene_->isRunning()) scene_->quit();
				scene_->release();
			}

			scene_ = temp_;
			temp_.reset();
			scene_->init();
		}

		if(scene_ && !scene_->isRunning())
		{
			scene_->release();
			scene_.reset();
		}

		if(!scene_) return false;

		scene_->update(d);

		gl::start_draw();

		scene_->render();

		gl::update();

		return true;
	}

	void Scene::doKeyboard(Controls key, bool pressed)
	{
		if(scene_ && scene_->isRunning()) scene_->input(key, pressed);
	}
}

