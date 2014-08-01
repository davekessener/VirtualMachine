#include <iostream>
#include <vector>
#include <memory>
#include "inc.h"
#include "gl.h"
#include "sdl.h"
#include "image.h"

#define MXT_WINDOWNAME "DaViewer"
#define MXT_WIDTH 1240
#define MXT_HEIGHT 720

typedef unsigned int uint;

void do_init(int, int);
bool do_update(int);
void do_keyboard(sdl::Controls, bool);
void do_mouse(uint, uint, int, int);

Image img_;
std::vector<std::string> files_;
std::vector<std::string>::const_iterator idx_;
bool running_ = true;

void incImage();
void decImage();
void loadImage();

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	std::vector<std::string>(argv + 1, argv + argc).swap(files_);
	idx_ = files_.cbegin();

	loadImage();

	sdl::set_init(&do_init);
	sdl::set_update(&do_update);
	sdl::set_input(&do_keyboard, &do_mouse);

	sdl::start(MXT_WINDOWNAME, MXT_WIDTH, MXT_HEIGHT);

	return 0;
}

void do_init(int w, int h)
{
	gl::init2d(w, h);
}

bool do_update(int d)
{
	gl::start_draw();

	if(!img_.loaded())
	{
		loadImage();
		img_.create();
		img_.bind();
	}

	img_.center(MXT_WIDTH, MXT_HEIGHT);

	gl::update();

	return running_;
}

void do_keyboard(sdl::Controls c, bool down)
{
	using sdl::Controls;

	if(down)
	{
		switch(c)
		{
			case Controls::UP:
				incImage();
				break;
			case Controls::DOWN:
				decImage();
				break;
			case Controls::ESCAPE:
				running_ = false;
				break;
			default:
				break;
		}
	}
}

void do_mouse(uint x, uint y, int dx, int dy)
{
}

void decImage(void)
{
	if(idx_ == files_.cbegin()) idx_ = files_.cend();
	--idx_;
	if(img_.loaded()) img_.erase();
}

void incImage(void)
{
	++idx_;
	if(idx_ == files_.cend()) idx_ = files_.cbegin();
	if(img_.loaded()) img_.erase();
}

void loadImage(void)
{
	img_.load(*idx_);
}

