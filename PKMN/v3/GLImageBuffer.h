#ifndef PKMN_RENDER_GLIMAGEBUFFER_H
#define PKMN_RENDER_GLIMAGEBUFFER_H

#include "common.h"
#include <set>

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class GLImageBuffer
			{
				public:
					static DWORD getImage(const void *, size_t);
					static DWORD updateImage(DWORD, const void *, size_t);
					static void deleteImage(DWORD);
				private:
					static GLImageBuffer& instance( );
				private:
					GLImageBuffer( ) { }
					~GLImageBuffer( ) noexcept;
				private:
					std::set<DWORD> imgs_;
				private:
					GLImageBuffer(const GLImageBuffer&) = delete;
					GLImageBuffer& operator=(const GLImageBuffer&) = delete;
			};
		}
	}
}

#endif

