#include <iostream>
#include <vector>
#include <cassert>
#include "World.h"
#include "gl.h"

struct World::Impl
{
	Impl(int w, int h, int l) : w_(w), h_(h), l_(l), r_(64), blocks_(w_ * h_ * l_) { }
	inline BYTE& at(int x, int y, int z) { return blocks_.at(getPos(x, y, z)); }
	inline BYTE at(int x, int y, int z) const { return blocks_.at(getPos(x, y, z)); }
	inline int getPos(int x, int y, int z) const
	{
		assert(x>=0&&(size_t)x<w_);
		assert(y>=0&&(size_t)y<h_);
		assert(z>=0&&(size_t)z<l_);
		return x + (y + z * h_) * w_;
	}
	void render(size_t, size_t, size_t) const;
	inline void render_z(float z) const
	{
		gl::draw_face3d( 0.0, 0.0, 1.0, 1.0,
						-0.5, -0.5, z,
						   z,   -z, z,
						 0.5,  0.5, z,
						  -z,    z, z);
	}
	inline void render_y(float y) const
	{
		gl::draw_face3d( 0.0, 0.0, 1.0, 1.0,
						-0.5, y, -0.5,
						  -y, y,    y,
						 0.5, y,  0.5,
						   y, y,   -y);
	}
	inline void render_x(float x) const
	{
		gl::draw_face3d( 0.0, 0.0, 1.0, 1.0,
						x, -0.5, -0.5, 
						x,    x,   -x, 
						x,  0.5,  0.5, 
						x,   -x,    x);
	}

	size_t w_, h_, l_;
	int r_;
	std::vector<BYTE> blocks_;
};

// # ===========================================================================

World::World(int w, int h, int z) : impl_(new Impl(w, h, z))
{
}

void World::setBlock(int x, int y, int z, BYTE id)
{
	impl_->at(x, y, z) = id;
}

BYTE World::getBlock(int x, int y, int z) const
{
	return impl_->at(x, y, z);
}

void World::render(int cx, int cy, int cz) const
{
	auto i = impl_->blocks_.cbegin();
	BYTE oid(0);

	for(size_t z = 0 ; z < impl_->l_ ; ++z)
	{
		for(size_t y = 0 ; y < impl_->h_ ; ++y)
		{
			for(size_t x = 0 ; x < impl_->w_ ; ++x)
			{
				if(static_cast<int>(x) - cx < impl_->r_ && static_cast<int>(y) - cy < impl_->r_ && static_cast<int>(z) - cz < impl_->r_)
				{
					if(BYTE id = *i)
					{
						gl::push();
							if(id != oid) gl::bind_texture(id);
							gl::translate(x + 0.5, y + 0.5, z + 0.5);
							impl_->render(x, y, z);
						gl::pop();
					}
				}
				++i;
			}
		}
	}
}

void World::Impl::render(size_t x, size_t y, size_t z) const
{
	if(x == 0      || !at(x - 1, y, z)) render_x(-0.5);
	if(x == w_ - 1 || !at(x + 1, y, z)) render_x( 0.5);
	if(y == 0      || !at(x, y - 1, z)) render_y(-0.5);
	if(y == h_ - 1 || !at(x, y + 1, z)) render_y( 0.5);
	if(z == 0      || !at(x, y, z - 1)) render_z(-0.5);
	if(z == l_ - 1 || !at(x, y, z + 1)) render_z( 0.5);
}

// # ===========================================================================

World::World(void) noexcept : impl_(nullptr)
{
}

World::World(const World& w) : impl_(w.impl_ ? new Impl(*w.impl_) : nullptr)
{
}

World::World(World&& w) noexcept : impl_(nullptr)
{
	swap(w);
}

World::~World(void) noexcept
{
	delete impl_;
}

World& World::operator=(const World& w)
{
	World t(w);
	swap(t);
	return *this;
}

World& World::operator=(World&& w) noexcept
{
	swap(w);
	return *this;
}

void World::swap(World& w) noexcept
{
	Impl *i(impl_);
	impl_ = w.impl_;
	w.impl_ = i;
}

