#include <iostream>
#include <vector>
#include <cassert>
#include "World.h"
//#include "gl.h"
#include "Tessellator.h"
#include "Manager.h"
#include "Block.h"

struct World::Impl
{
	Impl(int w, int h, int l) : w_(w), h_(h), l_(l), blocks_(w_ * h_ * l_) { }
	inline BYTE& at(int x, int y, int z) { return blocks_.at(getPos(x, y, z)); }
	inline BYTE at(int x, int y, int z) const { return blocks_.at(getPos(x, y, z)); }
	inline int getPos(int x, int y, int z) const
	{
		assert(x>=0&&x<w_);
		assert(y>=0&&y<h_);
		assert(z>=0&&z<l_);
		return x + (y + z * h_) * w_;
	}
	void render(int, int, int, const vec&, const Block&) const;
	inline void render_z(float z, int id) const
	{
		float u = (id % 2) * 8 / 16.0, v = (id / 2) * 8 / 16.0;
		Tessellator &tess(Tessellator::instance());
		tess.addVertexUV( z,  0.5, z, u,       v);
		tess.addVertexUV(-z,  0.5, z, u + 0.5, v);
		tess.addVertexUV(-z, -0.5, z, u + 0.5, v + 0.5);
		tess.addVertexUV( z, -0.5, z, u,       v + 0.5);
//		gl::draw_face3d( u, v, u + 0.5, v + 0.5,
//						   z,  0.5, z,
//						  -z,  0.5, z,
//						  -z, -0.5, z,
//						   z, -0.5, z);
	}
	inline void render_y(float y, int id) const
	{
		float u = (id % 2) * 8 / 16.0, v = (id / 2) * 8 / 16.0;
		Tessellator &tess(Tessellator::instance());
		tess.addVertexUV(-0.5, y, -0.5, u,       v);
		tess.addVertexUV(  -y, y,    y, u + 0.5, v);
		tess.addVertexUV( 0.5, y,  0.5, u + 0.5, v + 0.5);
		tess.addVertexUV(   y, y,   -y, u,       v + 0.5);
//		gl::draw_face3d( u, v, u + 0.5, v + 0.5,
//						-0.5, y, -0.5,
//						  -y, y,    y,
//						 0.5, y,  0.5,
//						   y, y,   -y);
	}
	inline void render_x(float x, int id) const
	{
		float u = (id % 2) * 8 / 16.0, v = (id / 2) * 8 / 16.0;
		Tessellator &tess(Tessellator::instance());
		tess.addVertexUV(x,  0.5, -x, u,       v);
		tess.addVertexUV(x,  0.5,  x, u + 0.5, v);
		tess.addVertexUV(x, -0.5,  x, u + 0.5, v + 0.5);
		tess.addVertexUV(x, -0.5, -x, u,       v + 0.5);
//		gl::draw_face3d( u, v, u + 0.5, v + 0.5,
//						x,  0.5, -x, 
//						x,  0.5,  x,
//						x, -0.5,  x, 
//						x, -0.5, -x);
	}

	int w_, h_, l_;
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

void World::render(const Frustum& frustum) const
{
	int r = frustum.getMaxDist();
	const vec &p(frustum.getPos());
	int X(static_cast<int>(p.dx)), 
		Y(static_cast<int>(p.dy)), 
		Z(static_cast<int>(p.dz));
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
	int xmin(max(0, X - r)), xmax(min(impl_->w_, X + r));
	int ymin(max(0, Y - r)), ymax(min(impl_->h_, Y + r));
	int zmin(max(0, Z - r)), zmax(min(impl_->l_, Z + r));
#undef max
#undef min

	Tessellator &tess(Tessellator::instance());

	tess.startDrawing();
	for(int z = zmin ; z < zmax ; ++z)
	{
		for(int y = ymin ; y < ymax ; ++y)
		{
			for(int x = xmin ; x < xmax ; ++x)
			{
				if(BYTE id = impl_->at(x, y, z))
				{
					if(frustum.isInside(vec(x + 0.5, y + 0.5, z + 0.5), 0.87))
					{
						tess.setOffset(x + 0.5, y + 0.5, z + 0.5);
						impl_->render(x, y, z, vec(x, y, z) - p, Manager::instance().getBlock(id));
					}
				}
			}
		}
	}
	tess.draw();
}

inline constexpr bool check(const vec& v1, const vec& v2) { return angle(v1, v2) >= 90; }
//inline constexpr bool check(const vec& v1, const vec& v2) { return true; }

void World::Impl::render(int x, int y, int z, const vec& v, const Block& b) const
{
	if(check(v, vec(-1, 0, 0)) && (x == 0      || !at(x - 1, y, z))) render_x(-0.5, b.getFace(Direction::LEFT));
	if(check(v, vec( 1, 0, 0)) && (x == w_ - 1 || !at(x + 1, y, z))) render_x( 0.5, b.getFace(Direction::RIGHT));
	if(check(v, vec(0, -1, 0)) && (y == 0      || !at(x, y - 1, z))) render_y(-0.5, b.getFace(Direction::BOTTOM));
	if(check(v, vec(0,  1, 0)) && (y == h_ - 1 || !at(x, y + 1, z))) render_y( 0.5, b.getFace(Direction::TOP));
	if(check(v, vec(0, 0, -1)) && (z == 0      || !at(x, y, z - 1))) render_z(-0.5, b.getFace(Direction::FRONT));
	if(check(v, vec(0, 0,  1)) && (z == l_ - 1 || !at(x, y, z + 1))) render_z( 0.5, b.getFace(Direction::BACK));
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

