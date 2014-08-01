#ifndef MANAGER_H
#define MANAGER_H

#include "inc.h"

struct Block;

class Manager
{
	public:
	struct img_t
	{
		DWORD id;
		int width, height;
	};

	public:
		static Manager& instance( );
		img_t loadTexture(const std::string&);
		const Block& getBlock(int) const;
	private:
		Manager( );
		Manager(const Manager&);
		Manager& operator=(const Manager&);
		~Manager( );
	private:
		struct Impl;
		Impl *impl_;
};

#endif

