#ifndef PKMN_RENDER_SHELPER_H
#define PKMN_RENDER_SHELPER_H

#include "../common.h"
#include <vector>
#include <map>

namespace pkmn
{
	namespace render
	{
		class SHelper
		{
			public:
				static void render(DWORD id, uint p, int x, int y) { instance().doRender(id, p, x, y); }
				static void registerSprite(DWORD id) { instance().doRegisterSprite(id); }
				static void freeSprite(DWORD id) { instance().doFreeSprite(id); }
			private:
				SHelper( );
				~SHelper( ) noexcept;
				std::vector<BYTE>::iterator getNext(BYTE b) { return img_.begin() + b * 512 * 24 * 4; }
				bool isRegistered(DWORD id) const { return idmap_.find(id) != idmap_.cend(); }
				void doRender(DWORD, uint, int, int) const;
				void doRegisterSprite(DWORD);
				void doFreeSprite(DWORD);
			private:
				std::map<DWORD, DWORD> idmap_;
				std::vector<BYTE> img_;
				DWORD id_;
				BYTE next_;
			private:
				static SHelper& instance( ) { static SHelper sh; return sh; }
		};
	}
}

#endif

