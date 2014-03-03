#include "OWSprite.h"

void OWSprite::walk(Direction d)
{
	if(isMoving()) return;

	Moving::walk(d);

	switch(d)
	{
		case Direction::UP:
			facing = 1;
			break;
		case Direction::DOWN:
			facing = 0;
			break;
		case Direction::LEFT:
			facing = 2;
			break;
		case Direction::RIGHT:
			facing = 3;
			break;
	}
}

