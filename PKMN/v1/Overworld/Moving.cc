#include "Moving.h"

void Moving::update(void)
{
	if(dx > 0) dx += speed;
	else if(dx < 0) dx -= speed;
	else if(dy > 0) dy += speed;
	else if(dy < 0) dy -= speed;

	if(dx >= 16) { ++x; dx = 0; ++steps; }
	else if(dx <= -16) { --x; dx = 0; ++steps; }
	else if(dy >= 16) { ++y; dy = 0; ++steps; }
	else if(dy <= -16) { --y; dy = 0; ++steps; }
}

void Moving::walk(Direction d)
{
	if(isMoving()) return;

	switch(d)
	{
		case Direction::UP:
			--dy;
			break;
		case Direction::DOWN:
			++dy;
			break;
		case Direction::LEFT:
			--dx;
			break;
		case Direction::RIGHT:
			++dx;
			break;
	}
}

