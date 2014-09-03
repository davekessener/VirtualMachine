#ifndef PKMN_EDITOR_SURFACE_ICONBUFFER_H
#define PKMN_EDITOR_SURFACE_ICONBUFFER_H

#include "../../common.h"

namespace editor
{
	namespace surface
	{
		class IconBuffer
		{
			public:
				static inline DWORD registerIcon(const std::string& s) { return instance().doRegisterIcon(s); }
				static inline void renderIcon(DWORD id, int x1, int y1, int x2, int y2)
					{ instance().doDrawIcon(id, x1, y1, x2, y2); }
			private:
				IconBuffer( );
				~IconBuffer( );
				DWORD doRegisterIcon(const std::string&);
				void doDrawIcon(DWORD, int, int, int, int) const;
				static IconBuffer& instance( ) { static IconBuffer ib; return ib; }
			private:
				struct Impl;
				Impl *impl_;
			private:
				IconBuffer(const IconBuffer&) = delete;
				IconBuffer& operator=(const IconBuffer&) = delete;
		};
	}
}

#endif

