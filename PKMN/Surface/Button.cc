#define _BUTTON_MAIN
#include "Button.h"
#undef _BUTTON_MAIN

namespace surface
{
	Button::Button(Image *i, int x, int y, onAction_fn a, const std::string& name, const std::string& o)
		: Surface(new SubImage(i, x, y, TILE_SIZE, TILE_SIZE)), action(a), state(NONE)
	{
		std::string first(o.length() ? name + FE : name + __B_STD_1 + FE);
		std::string second(o.length() ? o + FE : name + __B_STD_2 + FE);
	
		buttons[0] = WindowHandler::getIcon(first);
		buttons[1] = WindowHandler::getIcon(second);
	}
	
	Button::~Button(void)
	{
		WindowHandler::returnIcon(buttons[0]);
		WindowHandler::returnIcon(buttons[1]);
	}
	
	void Button::mouseDown(button_t b, int x, int y)
	{
		changeState(PRESSED);
	}
	
	void Button::mouseDrag(button_t b, int x, int y)
	{
		if(b == ::Button::LEFT)
		{
			changeState(hit(x, y) ? PRESSED : INACTIVE);
		}
	}
	
	void Button::mouseUp(button_t b, int x, int y)
	{
		changeState(hit(x, y) ? RELEASED : NONE);
	}
	
	void Button::draw(Image *dI)
	{
		dI->blit(buttons[state == PRESSED], Point(0, 0), Rect(0, 0, TILE_SIZE, TILE_SIZE));
	}
	
	void Button::changeState(button_state s)
	{
		if(state != s)
		{
			action(state = s);
			dirty();
		}
	}
}

