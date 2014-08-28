#ifndef SCENE_H
#define SCENE_H

#include <functional>
#include <stack>
#include "Misc/Timer.h"

#ifdef SCENE_MAIN
#include "Terminal.h"
#endif

class Scene
{
	typedef std::function<bool(int)> inputFn_t;

	public:
		virtual ~Scene( ) { }
		static void play(Scene *);
	protected:
		virtual void init( );
		virtual void suspend( );
		virtual void wake( );
		virtual void input(int) = 0;
		virtual void update() = 0;
		virtual void refresh( ) = 0;
		virtual void finalize( );
		void quit( );
		void pushInputFunction(inputFn_t);
	private:
		void run( );
		bool running;
		std::stack<inputFn_t> inputFns;
		Timer timer;
		static Scene *scene;

		static const int FRAME_RATE = 40;
};

#endif

