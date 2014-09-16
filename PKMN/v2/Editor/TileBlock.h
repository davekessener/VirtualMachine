#ifndef PKMN_EDITOR_TILEBLOCK_H
#define PKMN_EDITOR_TILEBLOCK_H

#include "../common.h"

namespace editor
{
	class TileBlock
	{
		public:
			explicit TileBlock(DWORD i = 0) : lid_(i), rid_(i) { }
			TileBlock(DWORD l, DWORD r) { set(l, r); }
			int width( ) const { return (rid_ % 64) - (lid_ % 64) + 1; }
			int height( ) const { return (rid_ / 64) - (lid_ / 64) + 1; }
			DWORD at(uint x, uint y) const { return lid_ + x + y * 64; }
			DWORD left( ) const { return lid_; }
			DWORD right( ) const { return rid_; }
			void setL(DWORD l) { set(l, rid_); }
			void setR(DWORD r) { set(lid_, r); }
			bool operator==(const TileBlock& tb) const { return lid_ == tb.lid_ && rid_ == tb.rid_; }
			bool operator!=(const TileBlock& tb) const { return !(*this == tb); }
			void set(DWORD l, DWORD r)
			{
				int x1(l % 64), y1(l / 64), x2(r % 64), y2(r / 64);
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
				lid_ = min(x1, x2) + min(y1, y2) * 64;
				rid_ = max(x1, x2) + max(y1, y2) * 64;
#undef max
#undef min
			}
		private:
			DWORD lid_, rid_;
	};
}

#endif

