#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <dav/noise.h>
#include <dav/gl.h>
#include <dav/sdl.h>
#include "simple_bmp.h"
#include "World.h"
#include "Frustum.h"
#include "Manager.h"

static const int SCREEN_WIDTH = 1240;
static const int SCREEN_HEIGHT = 720;
static const int WWIDTH = 1000, WHEIGHT = 100, WLENGTH = 1000;
static const float FOV(45.0), ZNEAR(1.0), ZFAR(100.0);

bool do_update(int);
void do_init(int, int);
void do_input(sdl::Controls, bool);
void mmotion(unsigned int, unsigned int, int, int);

void set_look(float, float);

std::set<sdl::Controls> keyPressed_;
Manager::img_t img_;
World world_;
Frustum frust_;
std::string imgName_;

float x_, y_, z_;
float vx_, vy_, vz_;
float sx_, sz_;
float pitch_, yaw_;
float fspeed_, sspeed_, vspeed_;

inline bool pressed(sdl::Controls c) { return keyPressed_.count(c); }

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	imgName_ = args.at(1);

	fspeed_ = sspeed_ = vspeed_ = 0;

	sdl::set_init(&do_init);
	sdl::set_update(&do_update);
	sdl::set_input(&do_input, &mmotion);
	sdl::start("4D", SCREEN_WIDTH, SCREEN_HEIGHT);

	return 0;
}

void set_look(float p, float y)
{
	while(p > 360) p -= 360;
	while(p < 0) p += 360;
	if(y > 90) y = 90;
	if(y < -90) y = -90;

	pitch_ = p;
	yaw_ = y;

	p /= 180.0 / 3.141;
	y /= 180.0 / 3.141;

	float xzl = cos(y);
	vx_ = sin(-p);
	vy_ = -sin(y);
	vz_ = cos(p);

	sx_ = -vz_;//sin(-(p + 3.141 / 2));
	sz_ = vx_;//cos(p + 3.141 / 2);

	float ux = -vy_ * vx_, uy = xzl, uz = -vy_ * vz_;

	frust_.setCam(vec(x_, y_, z_), vec(-vx_ * xzl, -vy_, -vz_ * xzl), vec(ux, uy, uz));

	if(!pressed(sdl::Controls::SHIFT))
	{
		vx_ *= xzl;
		vz_ *= xzl;
	}
}

void populateWorld( );

void do_init(int w, int h)
{
	gl::init3d(w, h, FOV, ZNEAR, ZFAR);

	frust_ = Frustum(FOV, w / (float)h, ZNEAR, ZFAR);

	x_ = 0;
	y_ = 105;
	z_ = 0;
	set_look(0, -90);

	img_ = Manager::instance().loadTexture(imgName_);

	populateWorld();

	sdl::trap_mouse();
}

void render(void)
{
	gl::start_draw();

	gl::push();

	gl::bind_texture(img_.id);

	gl::rotate(-yaw_, 1, 0, 0);
	gl::rotate(pitch_, 0, 1, 0);
	gl::translate(-x_, -y_, -z_);

	world_.render(frust_);

	gl::pop();

	gl::update();
}

bool do_update(int d)
{
	using sdl::Controls;

	static const float MAXSPEED = 0.5;

	float max_speed = pressed(Controls::SPACE) ? 15 * MAXSPEED : MAXSPEED;
	float accel = max_speed / 500.0;

	float ox = x_, oy = y_, oz = z_;

	if(pressed(Controls::S))
	{
		if(fspeed_ < max_speed) fspeed_ += accel * d;
	}
	if(pressed(Controls::W))
	{
		if(fspeed_ > -max_speed) fspeed_ -= accel * d;
	}

	if(!pressed(Controls::W) && !pressed(Controls::S))
	{
		fspeed_ = 0.0;
	}
	else
	{
		x_ += vx_ * fspeed_;
		if(!pressed(Controls::SHIFT)) y_ += vy_ * fspeed_;
		z_ += vz_ * fspeed_;
	}

	if(pressed(Controls::A))
	{
		if(sspeed_ < max_speed) sspeed_ += accel * d;
	}
	if(pressed(Controls::D))
	{
		if(sspeed_ > -max_speed) sspeed_ -= accel * d;
	}

	if(!pressed(Controls::A) && !pressed(Controls::D))
	{
		sspeed_ = 0.0;
	}
	else
	{
		x_ += sx_ * sspeed_;
		z_ += sz_ * sspeed_;
	}

	if(pressed(Controls::Q))
	{
		if(vspeed_ < max_speed) vspeed_ += accel * d;
	}
	if(pressed(Controls::E))
	{
		if(vspeed_ > -max_speed) vspeed_ -= accel * d;
	}

	if(!pressed(Controls::E) && !pressed(Controls::Q))
	{
		vspeed_ = 0.0;
	}
	else
	{
		y_ += vspeed_;
	}

	if(ox != x_ || oy != y_ || oz != z_)
	{
		frust_.updateCam(vec(x_, y_, z_));
	}
	
	render();

	return !pressed(Controls::ESCAPE);
}

void do_input(sdl::Controls key, bool pressed)
{
	if(pressed)
	{
		keyPressed_.insert(key);
	}
	else
	{
		auto i = keyPressed_.find(key);
		if(i != keyPressed_.end()) keyPressed_.erase(i);
	}

	if(pressed && key == sdl::Controls::RETURN)
	{
		const vec &p(frust_.getPos()), &v(frust_.getView()), &u(frust_.getUp());
		std::cout << "@[" << p.dx << ", " << p.dy << ", " << p.dz << "]: ("
				  << v.dx << ", " << v.dy << ", " << v.dz << ")"
				  << " x (" << u.dx << ", " << u.dy << ", " << u.dz << ")"
				  << " pitch: " << pitch_ << ", yaw: " << yaw_ << std::endl;
//		std::cout << frust_ << std::endl;
	}

	if(key == sdl::Controls::SHIFT) set_look(pitch_, yaw_);
}

void mmotion(unsigned int x, unsigned int y, int dx, int dy)
{
	set_look(pitch_ + dx * 90.0 / 1000.0, yaw_ - dy * 90.0 / 1000.0);
}

void populateWorld(void)
{
	lib::aux::Noise noise(0x64);

	world_ = World(WWIDTH, WHEIGHT, WLENGTH);

	for(int z = 0 ; z < WLENGTH ; ++z)
	{
		for(int x = 0 ; x < WWIDTH ; ++x)
		{
			float dx = x / (float)WWIDTH, dz = z / (float)WLENGTH;
			float r = noise.noise3d(dx * 8.0, 0.5, dz * 8.0);
			size_t h = static_cast<size_t>(WHEIGHT * (0.5 + 0.15 * r));

			for(size_t y = 0 ; y < h ; ++y)
			{
				world_.setBlock(x, y, z, 3);
			}

			world_.setBlock(x, h, z, 1);
		}
	}
}

