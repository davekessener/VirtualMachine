#include "WindowHandler.h"

WindowHandler *WindowHandler::_instance = NULL;

void WindowHandler::init(SDL_Renderer *r)
{
	if(_instance) throw SDLException("Attempted reinitialization of WindowHandler. Abort.");

	_instance = new WindowHandler(r);
}

void WindowHandler::end(void)
{
	if(!_instance) throw SDLException("Attempted shut down uninitialized WindowHandler. Abort.");

	delete _instance;
}

Image *WindowHandler::getIcon(const std::string& fn)
{
	return instance()._getIcon(fn);
}

void WindowHandler::returnIcon(Image *i)
{
	instance()._returnIcon(i);
}

Image *WindowHandler::_getIcon(const std::string& fn)
{
	if(locations.count(fn) == 0)
	{
		Image *i = new Image(render, fn);
		
		int id;
		if(empty_stack.empty())
		{
			id = nextPos++;
		}
		else
		{
			id = empty_stack.top();
			empty_stack.pop();
		}
		
		int x = (id % CX_ELEMS) << 4;
		int y = (id / CX_ELEMS) << 4;

		stitch->blit(i, Point(x, y), Rect(0, 0, 16, 16));

		delete i;

		i = new SubImage(stitch, x, y, 16, 16);

		icons[i] = std::make_pair(1, fn);
		locations[fn] = std::make_pair(id, i);

		return i;
	}
	else
	{
		Image *i = locations.at(fn).second;

		++icons.at(i).first;

		return i;
	}
}

void WindowHandler::_returnIcon(Image *i)
{
	if(!icons.count(i)) throw SDLException("Tried to return unknown image. Abort.");

	if(!--icons.at(i).first)
	{
		empty_stack.push(locations.at(icons.at(i).second).first);
		locations.erase(locations.find(icons.at(i).second));
		icons.erase(icons.find(i));
	}
}

WindowHandler::WindowHandler(SDL_Renderer *r) : render(r), nextPos(0)
{
	stitch = new Image(render, CX_ELEMS * 16, CY_ELEMS * 16);
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
	if(!_instance) throw SDLException("Attempted to access uninitialized WindowHandler. Abort.");

	return *_instance;
}

