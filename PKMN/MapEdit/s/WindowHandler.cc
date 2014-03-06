#include "WindowHandler.h"

Image *WindowHandler::getIcon(const std::string& fn)
{
	return instance()._getIcon(fn.substr(fn.length() - 4).compare(FE) != 0 ? fn + FE : fn);
}

void WindowHandler::returnIcon(Image *i)
{
	instance()._returnIcon(i);
}

Image *WindowHandler::_getIcon(const std::string& fn)
{
	LOG("Retrieving icon '%s' [...]", fn.c_str());

	if(locations.count(fn) == 0)
	{
		Image *i = new Image(fn);
		
		int id;
		if(empty_stack.empty())
		{
			id = nextPos++;
			LOG("\tNew ID: %d", id);
		}
		else
		{
			id = empty_stack.top();
			empty_stack.pop();
			LOG("\tReuses old ID: %d", id);
		}
		
		int x = (id % CX_ELEMS) * TILE_SIZE;
		int y = (id / CX_ELEMS) * TILE_SIZE;

		stitch->blit(i, Point(x, y), Rect(0, 0, TILE_SIZE, TILE_SIZE));

		delete i;

		i = new SubImage(stitch, x, y, TILE_SIZE, TILE_SIZE);

		icons[i] = std::make_pair(1, fn);
		locations[fn] = std::make_pair(id, i);

		LOG("[DONE]");

		return i;
	}
	else
	{
		Image *i = locations.at(fn).second;

		++icons.at(i).first;

		LOG("\tRaised used-counter to %d", icons.at(i).first);
		LOG("[DONE]");

		return i;
	}
}

void WindowHandler::_returnIcon(Image *i)
{
	if(!icons.count(i)) throw SDLException("Tried to return unknown image. Abort.");

	LOG("Returning icon #%d '%s' (%d references still out) [...]", 
		locations.at(icons.at(i).second).first, icons.at(i).second.c_str(), icons.at(i).first - 1);

	if(!--icons.at(i).first)
	{
		empty_stack.push(locations.at(icons.at(i).second).first);
		LOG("\tFree image and recycle id %d", empty_stack.top());
		locations.erase(locations.find(icons.at(i).second));
		icons.erase(icons.find(i));
	}

	LOG("[DONE]");
}

WindowHandler::WindowHandler(void) : nextPos(0)
{
	stitch = new Image(CX_ELEMS * TILE_SIZE, CY_ELEMS * TILE_SIZE);
}

WindowHandler::~WindowHandler(void)
{
	for(auto i = icons.begin() ; i != icons.end() ; ++i)
	{
		delete i->first;
	}

	delete stitch;
}

WindowHandler& WindowHandler::instance(void)
{
	static WindowHandler wh;
	return wh;
}

