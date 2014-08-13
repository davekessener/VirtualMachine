#ifndef WOLRD_H
#define WORLD_H

#include "inc.h"
#include "Frustum.h"

class World
{
	public:
		World( ) noexcept;
		World(const World&);
		World(World&&) noexcept;
		World(int, int, int);
		~World( ) noexcept;
		World& operator=(const World&);
		World& operator=(World&&) noexcept;
		void swap(World&) noexcept;
		void setBlock(int, int, int, BYTE);
		BYTE getBlock(int, int, int) const;
		void render(const Frustum&) const;
	private:
		struct Impl;
		Impl *impl_;
};

#endif

