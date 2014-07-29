#include "Block.h"

Block::Block(int id) : id_(id)
{
}

Block::~Block(void) noexcept
{
}

int Block::getFace(Direction d) const
{
	return id_ - 1;
}

