#include "Camera.h"

void Camera::update(void)
{
	if(_lock)
	{
		x = _lock->getX();
		y = _lock->getY();
		dx = _lock->getDX();
		dy = _lock->getDY();
	}
	else
	{
		Moving::update();
	}
}

void Camera::walk(Direction d)
{
	if(!_lock) Moving::walk(d);
}

void Camera::lock(const Moving *__lock)
{
	_lock = __lock;
	update();
}

