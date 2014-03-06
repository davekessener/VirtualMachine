#define _STATUSBAR_MAIN
#include "StatusBar.h"
#undef _STATUSBAR_MAIN

namespace surface
{
	StatusBar::StatusBar(Image *i, const std::string& t)
		: Surface(new SubImage(i, BORDER_SIZE, BORDER_SIZE, i->width() - 2 * BORDER_SIZE, BAR_SIZE), 
			BORDER_SIZE, BORDER_SIZE), title(t)
	{
		i = getDrawSurface();

		registerSurface(close = new Button(i, W() - (TILE_SIZE + 2), 2,
			[ ](Button::button_state b)
			{
				if(b == Button::RELEASED)
				{
					SDL_Event e;
					e.type = SDL_QUIT;
					SDL_PushEvent(&e);
				}
			}, __SB_CLOSE_BTN));
		registerSurface(minimize = new Button(i, W() - 2 * (TILE_SIZE + 2), 2,
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

	void StatusBar::draw(void)
	{
		Image *dI = getDrawSurface();

		dI->startBlit();
		dI->blit(bg, Point(0, 0), Rect(0, 0, W(), H()));
		dI->endBlit();
	}
}

