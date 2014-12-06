#ifndef DAV_PKMN_V3_SURFACE_TEXTENGINE_H
#define DAV_PKMN_V3_SURFACE_TEXTENGINE_H

#include <map>
#include "common.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class TextureEngine
			{
				public:
					static DWORD LoadTexture(const std::string& s) { return Instance().load(s); }
					static void DeleteTexture(DWORD id) { Instance().remove(id); }
				private:
					static TextureEngine& Instance( ) { static TextureEngine te; return te; }
					DWORD load(const std::string&);
					void remove(DWORD);
					DWORD generate(const std::string&);
				private:
					std::map<std::string, DWORD> lookup_;
					std::map<DWORD, std::string> reverse_;
			};
		}
	}
}

#endif

