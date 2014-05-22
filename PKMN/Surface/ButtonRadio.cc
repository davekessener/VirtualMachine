#include "ButtonRadio.h"

namespace surface
{
	ButtonRadio::ButtonRadio(Image *i, int x, int y, onAction_fn fn, const std::string& s1, const std::string& s2)
		: ButtonToggle(i, x, y, fn, s1, s2)
	{
	}

	void ButtonRadio::mouseDown(button_t b, int x, int y)
	{
		if(getState() != PRESSED)
		{
			for(ButtonRadio *b : group_)
			{
				b->changeState(RELEASED);
			}

			changeState(PRESSED);
		}
	}

	void ButtonRadio::connect(ButtonRadio& b)
	{
		group_.push_back(&b);
	}

	RadioButtonGroup& RadioButtonGroup::add(ButtonRadio *b)
	{
		btns_.push_back(b);

		return *this;
	}

	void RadioButtonGroup::join(void)
	{
		for(ButtonRadio *r : btns_)
		{
			for(ButtonRadio *i : btns_)
			{
				if(i == r) continue;

				r->connect(*i);
			}
		}

		btns_.clear();
	}
}

