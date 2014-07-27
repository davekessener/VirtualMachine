#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <image/noise.h>
#include "gl.h"
#include "sdl.h"
#include "simple_bmp.h"
#include "World.h"

using lib::img::simple::image;
using lib::img::simple::raw;

static const int SCREEN_WIDTH = 512;
static const int SCREEN_HEIGHT = 512;
static const size_t WWIDTH = 1000, WHEIGHT = 100, WLENGTH = 1000;


bool do_update(int);
void do_init(int, int);
void do_input(sdl::Controls, bool);
void mmotion(unsigned int, unsigned int, int, int);

void set_look(double, double);

DWORD id;
image img;
std::set<sdl::Controls> keyPressed;

World world;

double x, y, z;
double vx, vy, vz;
double sx, sz;
double pitch, yaw;
double fspeed, sspeed, vspeed;

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	using namespace lib::img::simple;

	img = image(read_simple_bmp(args.at(1)));

	x = WWIDTH / 2;
	y = WHEIGHT + 3;
	z = WLENGTH / 2;

	set_look(0, 0);

	fspeed = sspeed = vspeed = 0;

	sdl::set_init(&do_init);
	sdl::set_update(&do_update);
	sdl::set_input(&do_input, &mmotion);
	sdl::start("Testwindow", SCREEN_WIDTH, SCREEN_HEIGHT);

	return 0;
}

void set_look(double p, double y)
{
	while(p > 360) p -= 360;
	while(p < 0) p += 360;
	if(y > 90) y = 90;
	if(y < -90) y = -90;

	pitch = p;
	yaw = y;

	p /= 180.0 / 3.141;
	y /= 180.0 / 3.141;

	double xzl = cos(y);
	vx = sin(-p);
	vy = sin(y);
	vz = cos(p);

	sx = -vz;//sin(-(p + 3.141 / 2));
	sz = vx;//cos(p + 3.141 / 2);

	vx *= xzl;
	vz *= xzl;
}

void populateWorld( );

void do_init(int w, int h)
{
	gl::init3d(w, h);

	id = gl::create_texture(raw(img), img.width, img.height);

	populateWorld();
}

void render(void)
{
	gl::start_draw();

	gl::push();

	gl::rotate(yaw, 1, 0, 0);
	gl::rotate(pitch, 0, 1, 0);
	gl::translate(x, y, z);

	world.render(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));

	gl::pop();

	gl::update();
}

inline bool pressed(sdl::Controls c) { return keyPressed.count(c); }

bool do_update(int d)
{
	using sdl::Controls;

	static const double max_speed = 15.0 / 1000.0; // 5 blocks per second
	static const double accel = max_speed / 5.0; // max_speed reached in 3s

	if(pressed(Controls::UP))
	{
		if(fspeed < max_speed) fspeed += accel * d;
	}
	if(pressed(Controls::DOWN))
	{
		if(fspeed > -max_speed) fspeed -= accel * d;
	}

	if(!pressed(Controls::UP) && !pressed(Controls::DOWN))
	{
		fspeed = 0.0;
	}
	else
	{
		x += vx * fspeed;
		if(!pressed(Controls::SHIFT)) y += vy * fspeed;
		z += vz * fspeed;
	}

	if(pressed(Controls::RIGHT))
	{
		if(sspeed < max_speed) sspeed += accel * d;
	}
	if(pressed(Controls::LEFT))
	{
		if(sspeed > -max_speed) sspeed -= accel * d;
	}

	if(!pressed(Controls::LEFT) && !pressed(Controls::RIGHT))
	{
		sspeed = 0.0;
	}
	else
	{
		x += sx * sspeed;
		z += sz * sspeed;
	}

	if(pressed(Controls::ASCEND))
	{
		if(vspeed < max_speed) vspeed += accel * d;
	}
	if(pressed(Controls::DESCEND))
	{
		if(vspeed > -max_speed) vspeed -= accel * d;
	}

	if(!pressed(Controls::ASCEND) && !pressed(Controls::DESCEND))
	{
		vspeed = 0.0;
	}
	else
	{
		y += vspeed;
	}
	
	render();

	return !pressed(Controls::ESCAPE);
}

void do_input(sdl::Controls key, bool pressed)
{
	if(pressed)
	{
		keyPressed.insert(key);
	}
	else
	{
		auto i = keyPressed.find(key);
		if(i != keyPressed.end()) keyPressed.erase(i);
	}
}

void mmotion(unsigned int x, unsigned int y, int dx, int dy)
{
	set_look(pitch + dx * 90.0 / 1000.0, yaw + dy * 90.0 / 1000.0);
}

void populateWorld(void)
{
	lib::aux::Noise noise(0x64);

	world = World(WWIDTH, WHEIGHT, WLENGTH);

	for(size_t z = 0 ; z < WLENGTH ; ++z)
	{
		for(size_t x = 0 ; x < WWIDTH ; ++x)
		{
			double dx = x / (double)WWIDTH, dz = z / (double)WLENGTH;
			double r = noise.noise3d(dx * 8.0, 0.5, dz * 8.0);
			size_t h = static_cast<size_t>(WHEIGHT * (0.5 + r / 9.0));

			for(size_t y = 0 ; y < h ; ++y)
			{
				world.setBlock(x, y, z, id);
			}
		}
	}
}

