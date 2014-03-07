#define _STATUSBAR_MAIN
#include "StatusBar.h"
#undef _STATUSBAR_MAIN

namespace surface
{
	StatusBar::StatusBar(Image *i, int x, int y, int w, const std::string& t)
		: Surface(i = new SubImage(i, x, y, w, BAR_SIZE), x, y), title(t)
	{
		registerSurface(close = new Button(i, w - (TILE_SIZE + 2), 2,
			[ ](Button::button_state b)
			{
				if(b == Button::RELEASED)
				{
					SDL_Event e;
					e.type = SDL_QUIT;
					SDL_PushEvent(&e);
				}
			}, __SB_CLOSE_BTN));
		registerSurface(minimize = new Button(i, w - 2 * (TILE_SIZE + 2), 2,
			[ ](Button::button_state b)
			{
				if(b == Button::RELEASED)
				{
					SDL_Event e;
					e.type = SDL_WINDOWEVENT;
					e.window.event = SDL_WINDOWEVENT_MINIMIZED;
					SDL_PushEvent(&e);
				}
			}, __SB_MINIMIZE_BTN));

		bg = new Image(W(), H());
		bg->gradientFill(LIGHT, LIGHT, DARK, DARK);
	}

	StatusBar::~StatusBar(void)
	{
		delete close;
		delete minimize;
		delete bg;
	}

	void StatusBar::setTitle(const std::string& t)
	{
		title = t;
		dirty();
	}

	void StatusBar::draw(Image *dI)
	{
		dI->startBlit();
		dI->blit(bg, Point(0, 0), Rect(0, 0, W(), H()));
		// draw title && icon
		dI->endBlit();
	}
}

