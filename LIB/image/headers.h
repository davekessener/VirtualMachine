#ifndef LIB_IMAGE_HEADERS_H
#define LIB_IMAGE_HEADERS_H

#include <cstdint>

namespace lib
{
	namespace img
	{
		static const std::uint16_t BMP_MAGIC(0x4d42); // 'BM'
		static const std::uint16_t BMP_RES(0x0b13); // 2,835 px/m

		enum class Compression : std::uint32_t
		{
			RGB = 0,
			RLE8,
			RLE5,
			BITFIELDS,
			JPEG,
			PNG,
			ALPHABITFIELDS,
			CMYK = 11,
			CMYKRLE8,
			CMYKTLE4
		};

		struct bmp_header_t
		{
			std::uint16_t id;
			std::uint32_t filesize;
			std::uint32_t reserved;
			std::uint32_t offset;
		} __attribute__((packed));

		struct dib_OS21XBITMAPHEADER_t
		{
			std::uint32_t headersize;
			std::uint16_t width;
			std::uint16_t height;
			std::uint16_t planes;
			std::uint16_t bpp;
		} __attribute__((packed));
		typedef dib_OS21XBITMAPHEADER_t dib_BITMAPCOREHEADER_t;

		struct dib_OS22XBITMAPHEADER_t
		{
		};
		typedef dib_OS22XBITMAPHEADER_t dib_BITMAPCOREHEADER2_t;

		struct dib_BITMAPINFOHEADER_t
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
			std::uint32_t importantcolors;
		} __attribute__((packed));

		struct dib_BITMAPV2INFOHEADER_t
		{
		};

		struct dib_BITMAPV3INFOHEADER_t
		{
		};

		struct dib_BITMAPV4HEADER_t
		{
		};

		struct dib_BITMAPV5HEADER_t
		{
		};
	}
}

#endif

