#include "ButtonToggle.h"

namespace surface
{
	void ButtonToggle::mouseDown(button_t b, int x, int y)
	{
		if(getState() == PRESSED)
		{
			changeState(RELEASED);
		}
		else
		{
			changeState(PRESSED);
		}
	}

	void ButtonToggle::mouseDrag(button_t b, int x, int y)
	{
	}

	void ButtonToggle::mouseUp(button_t b, int x, int y)
	{
	}
}

