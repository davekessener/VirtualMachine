#ifndef DAV_PKMN_V3_SURFACE_TEXTSTITCHER_H
#define DAV_PKMN_V3_SURFACE_TEXTSTITCHER_H

#include <string>
#include <map>
#include "common.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class TextureStitcher
			{
				public:
					static text_info LoadIcon(const std::string& s) { return Instance().doLoadIcon(s); }
				private:
					text_info doLoadIcon(const std::string&);
				private:
					static TextureStitcher& Instance( ) { static TextureStitcher ts; return ts; }
				private:
					struct Stitcher;
					std::map<std::string, std::pair<text_info, DWORD>> lookup_;
					std::map<DWORD, Stitcher *> st_;
				private:
					TextureStitcher( ) = default;
					~TextureStitcher( );
					TextureStitcher(const TextureStitcher&) = delete;
					TextureStitcher& operator=(const TextureStitcher&) = delete;
			};
		}
	}
}

#endif

