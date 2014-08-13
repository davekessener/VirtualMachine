#ifndef LIB_BMPIMAGE_H
#define LIB_BMPIMAGE_H

#include <iosfwd>
#include <cstdint>

namespace lib
{
	namespace img
	{
		enum class ColorType : size_t
		{
			RGB = 3,
			RGBA = 4
		};

		class bmp_image
		{
			public:
				bmp_image( );
				bmp_image(const bmp_image&);
				bmp_image(bmp_image&&);
				~bmp_image( );
				bmp_image& operator=(const bmp_image&);
				bmp_image& operator=(bmp_image&&);
				void swap(bmp_image&) noexcept;

				bmp_image(std::istream&);
				bmp_image(std::uint8_t *, size_t width, size_t height, ColorType = ColorType::RGB);
				bmp_image(std::uint32_t *d, size_t w, size_t h, ColorType ct = ColorType::RGBA)
					: bmp_image(reinterpret_cast<std::uint8_t *>(d), w, h, ct) { }
				void toFile(std::ostream&) const;
				const std::uint8_t *getRawData( ) const;
				std::uint8_t *getRawData( );
				ColorType getColorType( ) const;
				size_t getWidth( ) const;
				size_t getHeight( ) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

