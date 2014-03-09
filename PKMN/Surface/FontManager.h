#ifndef _FONTMANAGER_H
#define _FONTMANAGER_H

#include <map>
#include <SDL_ttf.h>
#include "Geometry.h"

class FontManager
{
	public:
		static TTF_Font *getFont(const std::string&, bool = false, int = 8);
		static void end( ) { delete instance; instance = NULL; }
	private:
		FontManager( );
		~FontManager( );
		TTF_Font *_getFont(const std::string&, bool, int);
		std::map<std::pair<std::string, int>, TTF_Font *> fonts;

		static FontManager *instance;
};

#endif

