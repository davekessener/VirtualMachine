#ifndef PKMN_CAMERA_H
#define PKMN_CAMERA_H

#include "common.h"
#include "Locking.h"

namespace pkmn
{
	class Camera
	{
		public:
			Camera( ) = default;
			void lock(const Locking& l) { lock_ = &l; }
			point getOffset( ) const;
		private:
			const Locking *lock_;
			point pos_;
	};
}

#endif

