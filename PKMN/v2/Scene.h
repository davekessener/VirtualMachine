#ifndef PKMN_SCENE_H
#define PKMN_SCENE_H

#include "common.h"
#include <string>
#include <vector>
#include <memory>
#include <dav/sdl.h>

namespace pkmn
{
	class Scene : public std::enable_shared_from_this<Scene>
	{
		public:
		typedef std::shared_ptr<Scene> Scene_ptr;
		typedef std::weak_ptr<Scene> Scene_wptr;
		typedef dav::sdl::Controls Controls;

		public:
			static int run(const params_t&);
		protected:
			Scene( ) : running_(true) { }
			void quit( ) { i_doQuit(); running_ = false; }
			bool isRunning( ) const { return running_; }
			static void change(Scene_ptr p) { temp_ = p; }
		private:
			virtual void init( ) { }
			virtual void update(int) = 0;
			virtual void input(Controls, bool) = 0;
			virtual void render( ) const = 0;
			virtual void release( ) { }
		private:
			virtual void i_doQuit( ) { }
		private:
			static void doInit(int, int);
			static bool doUpdate(int);
			static void doKeyboard(Controls, bool);
		private:
			bool running_;
		private:
			static Scene_ptr scene_, temp_;
			static int width_, height_;
	};

	typedef Scene::Scene_ptr Scene_ptr;
}

#endif

