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
					static DWORD GetImage(const void *, size_t);
					static DWORD UpdateImage(DWORD, const void *, size_t);
					static void SetNear(DWORD);
					static void SetLinear(DWORD);
					static void DeleteImage(DWORD);
				private:
					static GLImageBuffer& Instance( );
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

