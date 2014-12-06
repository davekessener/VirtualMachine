#include "TextEngine.h"
#include "PNGLoader.h"

namespace dav { namespace pkmn { namespace screen {

DWORD TextureEngine::load(const std::string& s)
{
	auto i(lookup_.find(s));

	if(i != lookup_.end()) return i->second;

	DWORD id = PNGLoader::LoadPNG(s);

	lookup_[s] = id;
	reverse_[id] = s;

	return id;
}

void TextureEngine::remove(DWORD id)
{
	auto i(reverse_.find(id));

	if(i == reverse_.end()) return;

	lookup_.erase(lookup_.find(i->second));

	reverse_.erase(i);
}

}}}

