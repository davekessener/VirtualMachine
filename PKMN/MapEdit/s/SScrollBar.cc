#include "SScrollBar.h"

SScrollBar::SScrollBar(Image *i, int x, int y, int w, int h, int r, names_arr names)
	: Surface(new SubImage(i, x, y, w, h), x, y), range(r), position(0)
{
	cursor[0] = WindowHandler::getIcon(names[0]);
	cursor[1] = WindowHandler::getIcon(names[1]);
	fill = WindowHandler::getIcon(names[2]);

	registerSurface(incr = new SButton(i, 0, 0, 
		[this](SButton::button_state s)
			{
				if(s == SButton::RELEASED) setPosition(position + 1);
			}, names[3]));
	registerSurface(decr = new SButton(i, w - TILE_SIZE, h - TILE_SIZE,
		[this](SButton::button_state s)
			{
				if(s == SButton::RELEASED) setPosition(position - 1);
			}, names[4]));
}

SScrollBar::~SScrollBar(void)
{
	delete incr;
	delete decr;

	WindowHandler::returnIcon(cursor[0]);
	WindowHandler::returnIcon(cursor[1]);
	WindowHandler::returnIcon(fill);
}

void SScrollBar::mouseDown(button_t b, int x, int y)
{
	mouseDrag(b, x, y);
}

void SScrollBar::calculatePosition(int p, int r)
{
	r -= 2 * TILE_SIZE;
	p -= TILE_SIZE;

	if(p >= 0 && p < r) setPosition((range * p + r / 2) / r);
}

void SScrollBar::draw(void)
{
	Rect r(0, 0, TILE_SIZE, TILE_SIZE);
	Image *dI = getDrawSurface();

	dI->startBlit();

	for(int y = 0 ; y < _h - TILE_SIZE + 1 ; y += TILE_SIZE)
	{
		for(int x = 0 ; x < _w - TILE_SIZE + 1 ; x += TILE_SIZE)
		{
			dI->blit(fill, Point(x, y), r);
		}
	}

	if(range > 0)
	{
		int dx[2], dy[2];
		dx[0] = std::min(_w - TILE_SIZE, std::max(0, (_w - 4 * TILE_SIZE) * position / range) + TILE_SIZE);
		dy[0] = std::min(_h - TILE_SIZE, std::max(0, (_h - 4 * TILE_SIZE) * position / range) + TILE_SIZE);
		dx[1] = std::min(_w - TILE_SIZE, dx[0] + TILE_SIZE);
		dy[1] = std::min(_h - TILE_SIZE, dy[0] + TILE_SIZE);

		for(int i = 0 ; i < 2 ; ++i)
		{
			dI->blit(cursor[i], Point(dx[i], dy[i]), r);
		}
	}

	dI->endBlit();
}

void SScrollBar::setPosition(int p)
{
	if(position != p && p >= 0 && p < range)
	{
		position = p;
		dirty();
	}
}

