#include "FontManager.h"

FontManager *FontManager::instance = NULL;

TTF_Font *FontManager::getFont(const std::string& name, bool bold, int size)
{
	if(!instance) instance = new FontManager;

	return instance->_getFont(std::string("./fonts/") + name, bold, size);
}

FontManager::FontManager(void)
{
}

FontManager::~FontManager(void)
{
	for(auto i = fonts.begin() ; i != fonts.end() ; ++i)
	{
		TTF_CloseFont(i->second);
	}
	fonts.clear();
}

TTF_Font *FontManager::_getFont(const std::string& base, bool bold, int size)
{
	std::string name(base);
	if(bold) name += "bd";
	name += ".ttf";

	if(size <= 0) throw SDLException("ERR: Invalid font size (%d)", size);

	auto key = std::make_pair(name, size);

	if(fonts.count(key) == 0)
	{
		TTF_Font *f = TTF_OpenFont(name.c_str(), size);

		if(!f) throw SDLException(TTF_GetError());

		fonts[key] = f;
	}

	return fonts.at(key);
}

