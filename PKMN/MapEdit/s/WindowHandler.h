#ifndef _WINDOWHANDLER_H
#define _WINDOWHANDLER_H

#include <string>
#include <stack>
#include <map>
#include "Image.h"
#include "SubImage.h"

class WindowHandler
{
	public:
		static void init(SDL_Renderer *);
		static void end( );
		static Image *getIcon(const std::string&);
		static void returnIcon(Image *);
	private:
		WindowHandler(SDL_Renderer *);
		WindowHandler(const WindowHandler&);
		~WindowHandler( );
		WindowHandler& operator=(const WindowHandler&);
		Image *_getIcon(const std::string&);
		void _returnIcon(Image *);

		static WindowHandler& instance( );
		static WindowHandler *_instance;

		SDL_Renderer *render;
		std::map<Image *, std::pair<int, std::string>> icons;
		std::map<std::string, std::pair<int, Image *>> locations;
		std::stack<int> empty_stack;
		Image *stitch;
		int nextPos;

		static const int CX_ELEMS = 64, CY_ELEMS = 64;
};

#endif

