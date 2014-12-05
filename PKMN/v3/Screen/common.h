#ifndef DAV_PKMN_COMMON_H
#define DAV_PKMN_COMMON_H

#include <cstdint>
#include <cstddef>

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
typedef decltype(NULL) NULL_t;

typedef unsigned uint;

typedef std::uint8_t BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::uint64_t QWORD;

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

