#define _SBUTTON_MAIN
#include "SButton.h"
#undef _SBUTTON_MAIN

SButton::SButton(Image *i, int x, int y, onAction_fn a, const std::string& name, const std::string& o)
	: Surface(new SubImage(i, x, y, TILE_SIZE, TILE_SIZE), x, y), action(a), state(NONE)
{
	std::string first(o.length() ? name + FE : name + __SB_STD_1 + FE);
	std::string second(o.length() ? o + FE : name + __SB_STD_2 + FE);

	buttons[0] = WindowHandler::getIcon(first);
	buttons[1] = WindowHandler::getIcon(second);
}

SButton::~SButton(void)
{
	WindowHandler::returnIcon(buttons[0]);
	WindowHandler::returnIcon(buttons[1]);
}

void SButton::mouseDown(button_t b, int x, int y)
{
	changeState(PRESSED);
}

void SButton::mouseDrag(button_t b, int x, int y)
{
	changeState(hit(x, y) ? PRESSED : INACTIVE);
}

void SButton::mouseUp(button_t b, int x, int y)
{
	changeState(hit(x, y) ? RELEASED : NONE);
	state = NONE;
}

void SButton::draw(void)
{
	getDrawSurface()->blit(buttons[state == PRESSED], Point(0, 0), Rect(0, 0, TILE_SIZE, TILE_SIZE));
}

void SButton::changeState(button_state s)
{
	if(state != s)
	{
		action(state = s);
		dirty();
	}
}

