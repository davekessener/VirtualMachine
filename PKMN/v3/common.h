#ifndef DAV_PKMN_COMMON_H
#define DAV_PKMN_COMMON_H

#include <dav/inc.h>

#define NBT_NO_GZ
#include <nbt/NBT.h>

// # ===========================================================================

#define MXT_ICON_BASEUP "icons/btn_up.png"
#define MXT_ICON_BASEDOWN "icons/btn_down.png"

// # ===========================================================================

namespace dav
{
#define VPAIR(cn, ft, fn, st, sn) \
struct cn \
{ \
	cn( ) : fn(), sn() { } \
	cn(ft fn##_, st sn##_) : fn(fn##_), sn(sn##_) { } \
	ft fn; \
	st sn; \
};

	VPAIR(coords, int, x, int, y);
	VPAIR(size, int, w, int, h);
	VPAIR(bbox, coords, pos, size, vec);

	struct text_info
	{
		DWORD id;
		float u1, v1, u2, v2;
		uint size;
	};

	class color_t
	{
		public:
			color_t(DWORD c) { v_.color = c; }
			color_t(BYTE r, BYTE g, BYTE b, BYTE a = 0xff)
				{ v_.rgba.red = r; v_.rgba.green = g; v_.rgba.blue = b; v_.rgba.alpha = a; }
			operator DWORD( ) const { return v_.color; }
			BYTE red( ) const { return v_.rgba.red; }
			BYTE green( ) const { return v_.rgba.green; }
			BYTE blue( ) const { return v_.rgba.blue; }
			BYTE alpha( ) const { return v_.rgba.alpha; }
			DWORD rgb( ) const { return v_.color & 0x00ffffff; }
		private:
			union
			{
				DWORD color;
				struct
				{
					BYTE red, green, blue, alpha;
				} rgba;
			} v_;
	};
}

#endif

