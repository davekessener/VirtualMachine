#ifndef BLOCK_GRASS_H
#define BLOCK_GRASS_H

#include "Block.h"

class BlockGrass : public Block
{
	public:
		BlockGrass(int);
		virtual int getFace(Direction) const;
	private:
};

#endif

