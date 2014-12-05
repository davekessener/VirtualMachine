#include <vector>
#include "Manager.h"
#include "Surface.h"
#include "sdl.h"
#include "gl.h"

#define MXT_SCREENWIDTH (1920-2)
#define MXT_SCREENHEIGHT (1080-54)

namespace dav { namespace pkmn { namespace screen {

namespace
{
	bool running_ = false;
	uint time_ = 0;
	Surface_ptr root_;

	void init(int w, int h)
	{
		running_ = true;

		gl::init2d(w, h);

		root_->init(0, 0, w, h);
	}

	bool update(int d)
	{
		root_->update(d);

		if((time_ += d) >= 250)
		{
			root_->invalidate();
			time_ -= 250;
		}

		if(root_->dirty())
		{
			gl::start_draw();

			root_->render();

			gl::update();
		}

		return running_;
	}

	void input(sdl::Controls k, bool down)
	{
		if(down && k == sdl::Controls::ESCAPE) running_ = false;
	}

	void mouse(sdl::MouseButtons b, uint x, uint y, bool down)
	{
	}

	void mouse_m(uint x, uint y, int dx, int dy)
	{
	}

	void mouse_w(int dx, int dy)
	{
	}
}

void runProgram(const std::string& s, Surface_ptr p)
{
	root_ = p;

	sdl::set_init(init);
	sdl::set_update(update);
	sdl::set_input(input);
	sdl::set_mouse(mouse, mouse_m, mouse_w);

	sdl::start(s, MXT_SCREENWIDTH, MXT_SCREENHEIGHT);
}

}}}
