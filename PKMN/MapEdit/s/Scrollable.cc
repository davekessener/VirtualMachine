#include "Scrollable.h"

namespace surface
{
	Scrollable::Scrollable(Image *i, int x, int y, int w, int h) : Surface(new SubImage(i, x, y, w, h))
	{
	}

	Scrollable::~Scrollable(void)
	{
	}

	void Scrollable::draw(Image *i)
	{
	}
}

