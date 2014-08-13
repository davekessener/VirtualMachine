#include "BlockGrass.h"

BlockGrass::BlockGrass(int id) : Block(id)
{
}

int BlockGrass::getFace(Direction d) const
{
	switch(d)
	{
		case Direction::TOP:
			return 0;
		case Direction::BOTTOM:
			return 2;
		default:
			return 3;
	}
}

