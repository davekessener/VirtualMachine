#include "ButtonToggle.h"

namespace surface
{
	ButtonToggle::ButtonToggle(Image *i, int x, int y, onAction_fn fn, const std::string& s1, const std::string& s2)
				: Button(i, x, y, fn, s1, s2)
	{
	}

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

