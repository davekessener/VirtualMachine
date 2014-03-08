#include "ScrollBar.h"

namespace surface
{
	ScrollBar::ScrollBar(Image *i, int x, int y, int w, int h, int r, names_arr names)
		: Surface(i = new SubImage(i, x, y, w, h)), range(r), position(0)
	{
		cursor[0] = WindowHandler::getIcon(names[0]);
		cursor[1] = WindowHandler::getIcon(names[1]);
		fill = WindowHandler::getIcon(names[2]);
	
		registerSurface(decr = new Button(i, 0, 0, 
			[this](Button::button_state s)
				{
					if(s == Button::RELEASED) setPosition(position - 1);
				}, names[3]));
		registerSurface(incr = new Button(i, w - TILE_SIZE, h - TILE_SIZE,
			[this](Button::button_state s)
				{
					if(s == Button::RELEASED) setPosition(position + 1);
				}, names[4]));
	}
	
	ScrollBar::~ScrollBar(void)
	{
		delete incr;
		delete decr;
	
		WindowHandler::returnIcon(cursor[0]);
		WindowHandler::returnIcon(cursor[1]);
		WindowHandler::returnIcon(fill);
	}
	
	void ScrollBar::mouseDown(button_t b, int x, int y)
	{
		mouseDrag(b, x, y);
	}
	
	void ScrollBar::calculatePosition(int p, int r)
	{
		r -= 2 * TILE_SIZE;
		p -= TILE_SIZE;
	
		if(p >= 0 && p < r) setPosition((range * p + r / 2) / r);
	}
	
	void ScrollBar::draw(Image *dI)
	{
		Rect r(0, 0, TILE_SIZE, TILE_SIZE);
	
		dI->startBlit();
	
		for(int y = 0 ; y < H() - TILE_SIZE + 1 ; y += TILE_SIZE)
		{
			for(int x = 0 ; x < W() - TILE_SIZE + 1 ; x += TILE_SIZE)
			{
				dI->blit(fill, Point(x, y), r);
			}
		}
	
		if(range > 1)
		{
			int dx[2], dy[2];
			dx[0] = std::min(W() - TILE_SIZE, std::max(0, (W() - 4 * TILE_SIZE) * position / range) + TILE_SIZE);
			dy[0] = std::min(H() - TILE_SIZE, std::max(0, (H() - 4 * TILE_SIZE) * position / range) + TILE_SIZE);
			dx[1] = std::min(W() - TILE_SIZE, dx[0] + TILE_SIZE);
			dy[1] = std::min(H() - TILE_SIZE, dy[0] + TILE_SIZE);
	
			for(int i = 0 ; i < 2 ; ++i)
			{
				dI->blit(cursor[i], Point(dx[i], dy[i]), r);
			}
		}
	
		dI->endBlit();
	}
	
	void ScrollBar::setPosition(int p)
	{
		if(position != p && p >= 0 && p <= range)
		{
			position = p;
			dirty();
		}
	}
}

