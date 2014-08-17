#ifndef LIB_BMPHEADER_H
#define LIB_BMPHEADER_H

#include <cstdint>

namespace dav
{
	namespace img
	{
		static const std::uint16_t BMP_MAGIC(0x4d42); // 'BM'
		static const std::uint16_t BMP_RES(0x0b13); // 2,835 px/m

		struct bmp_header_t
		{
			std::uint16_t id;
			std::uint32_t filesize;
			struct
			{
				std::uint16_t reserved1;
				std::uint16_t reserved2;
			} __attribute__((packed)) reserved;
			std::uint32_t offset;
		} __attribute__((packed));

		struct dib_header_t
		{
			std::uint32_t headersize;
			std::int32_t width;
			std::int32_t height;
			std::uint16_t planes;
			std::uint16_t bpp;
			std::uint32_t compression;
			std::uint32_t size;
			struct
			{
				std::int32_t dx;
				std::int32_t dy;
			} __attribute__((packed)) resolution;
			std::uint32_t palettecount;
			std::uint32_t reserved;
		} __attribute__((packed));
	}
}

#endif

