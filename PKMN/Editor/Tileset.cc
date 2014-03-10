#include "Tileset.h"

namespace editor
{
	Tileset::Tileset(Image *i, int x, int y, int w, int h) : surface::Scrollable(i, x, y, w, h), bgColor(0xff00ffff), bgPlain(false), select(0)
	{
		tileset = new Image("./tileset.png");
		bg = new Image(W(), H());
		grid = new Image(W(), H());
	
		createBg();
		grid->drawGrid(0x000000ff, TILE_SIZE, TILE_SIZE);
	}
	
	Tileset::~Tileset(void)
	{
		delete bg;
		delete tileset;
	}
	
	void Tileset::mouseDown(button_t b, int x, int y)
	{
		x -= X(); y -= Y();
	
		select = (y / TILE_SIZE) * (tileset->width() / TILE_SIZE) + (x / TILE_SIZE);
		dirty();
	}
	
	void Tileset::draw(Image *dI, int dx, int dy)
	{
		bool showGrid = Settings::getBool(Settings::TS_GRID);

		dI->startBlit();
	
		if(bgPlain)
		{
			dI->fillRect(bgColor, 0, 0, dI->width(), dI->height());
		}
		else
		{
			dI->blit(bg, Point(0, 0), Rect(0, 0, W(), H()));
		}
	
		dI->blit(tileset, Point(0, 0), Rect(dx * TILE_SIZE, dy * TILE_SIZE, W(), H()));
		
		if(showGrid)
		{
			dI->blit(grid, Point(0, 0), Rect(0, 0, W(), H()));
		}
	
		dI->drawRect(color_rgba_t(255, 0, 0), 
		((select % (tileset->width() / TILE_SIZE)) - dx) * TILE_SIZE + (showGrid ? 1 : 0), 
		((select / (tileset->width() / TILE_SIZE)) - dy) * TILE_SIZE + (showGrid ? 1 : 0), 
		TILE_SIZE - (showGrid ? 1 : 0), TILE_SIZE - (showGrid ? 1 : 0));
		dI->endBlit();
	}
	
	std::pair<int, int> Tileset::getScrollRanges(void)
	{
		return std::make_pair((tileset->width() - W() + TILE_SIZE - 1) / TILE_SIZE, (tileset->height() - H() + TILE_SIZE - 1) / TILE_SIZE);
	}
	
	void Tileset::createBg(void)
	{
		bg->startBlit();
	
		bg->fillRect(bgColor, 0, 0, bg->width(), bg->height());
		
		if(!bgPlain);
		{
			color_rgba_t c = bgColor.inverse();
	
			for(int y = 0 ; y < bg->height() ; y += 2 * TILE_SIZE)
			{
				for(int x = 0 ; x < bg->width() ; x += 2 * TILE_SIZE)
				{
					bg->fillRect(c, x, y, TILE_SIZE, TILE_SIZE);
					bg->fillRect(c, x + TILE_SIZE, y + TILE_SIZE, TILE_SIZE, TILE_SIZE);
				}
			}
		}
	
		bg->endBlit();
	}
}

