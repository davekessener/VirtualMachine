#ifndef LIB_IMAGE_IO_H
#define LIB_IMAGE_IO_H

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>
#include "headers.h"
#include "image.hpp"
#include "aux.hpp"

namespace lib
{
	namespace img
	{
		template<typename DIB, typename I>
		class BMP
		{
			typedef typename I::value_type Color_t;
			typedef typename Color_t::value_type Data_t;
			typedef typename I::const_iterator iiter;

			public:
				BMP(const DIB&, const I&);
				void out(std::ostream&) const;
			private:
				bmp_header_t bmp_h_;
				DIB dib_h_;
				iiter i_;
				unsigned int rowsize;
		};

		template<typename DIB, typename I>
		BMP<DIB, I> create_bmp(const DIB& dib, const I& i)
		{
			return BMP<DIB, I>(dib, i);
		}

		template<typename DIB, typename I>
		inline std::ostream& operator<<(std::ostream& os, const BMP<DIB, I>& bmp)
		{
			bmp.out(os);
			return os;
		}

		template<typename DIB, typename I>
		BMP<DIB, I>::BMP(const DIB& dib, const I& i) : dib_h_(dib), i_(i.cbegin())
		{
			dib_h_.headersize = sizeof(dib_h_);
			dib_h_.planes = 1;
			dib_h_.bpp = sizeof(Color_t) * 8;
			dib_h_.palettecount = 0;
			dib_h_.importantcolors = 0;

			rowsize = ((dib_h_.bpp * dib_h_.width + 31) / 32) * 4;

			bmp_h_.id = BMP_MAGIC;
			bmp_h_.reserved = 0;
			bmp_h_.offset = sizeof(bmp_h_) + sizeof(dib_h_);
			bmp_h_.filesize = bmp_h_.offset + rowsize * dib_h_.height;
		}

		template<typename DIB, typename I>
		void BMP<DIB, I>::out(std::ostream& os) const
		{
			unsigned int l = dib_h_.width * sizeof(Color_t);
			std::vector<Color_t> buf(dib_h_.width);
			std::vector<std::uint8_t> pad(rowsize - l);
			iiter i(i_);

			os << aux::write_to(bmp_h_);
			os << aux::write_to(dib_h_);

			for(int h = 0 ; h < dib_h_.height ; ++h)
			{
				aux::transform_n(i, dib_h_.width, buf.begin(), [](const Color_t& c) { return static_cast<Data_t>(c); });
				os << aux::write_to(buf[0], l);
				os << aux::write_to(pad[0], pad.size());
			}
		}


//		template<typename I>
//		void writeBMP(std::ostream& os, dib_BITMAPINFOHEADER_t dib_h, const I& img)
//		{
//			typedef typename I::value_type C_t;
//			typedef typename C_t::value_type R_t;
//
//			dib_h.headersize = sizeof(dib_h);
//			dib_h.width = img.width();
//			dib_h.height = img.height();
//			dib_h.planes = 1;
//			dib_h.bpp = sizeof(C_t) * 8;
//			dib_h.compression = static_cast<std::uint32_t>(Compression::RGB);
//			dib_h.size = 0;
//			dib_h.resolution.dx = dib_h.resolution.dy = BMP_RES;
//			dib_h.palettecount = 0;
//			dib_h.importantcolors = 0;
//
//			std::uint32_t rowsize = ((dib_h.bpp * dib_h.width + 31) / 32) * 4;
//
//			bmp_header_t bmp_h;
//			bmp_h.id = BMP_MAGIC;
//			bmp_h.reserved = 0;
//			bmp_h.offset = sizeof(bmp_h) + sizeof(dib_h);
//			bmp_h.filesize = bmp_h.offset + rowsize * dib_h.height;
//
//			unsigned int l = dib_h.width * sizeof(C_t);
//			std::vector<C_t> buf(dib_h.width);
//			std::vector<std::uint8_t> pad(rowsize - l);
//			typename I::const_iterator i(img.cbegin());
//
//			os.write(reinterpret_cast<const char *>(&bmp_h), sizeof(bmp_h));
//			os.write(reinterpret_cast<const char *>(&dib_h), sizeof(dib_h));
//
//			for(int h = 0 ; h < dib_h.height ; ++h)
//			{
//				aux::transform_n(i, dib_h.width, buf.begin(), [](const C_t& c) { return static_cast<R_t>(c); });
//				os.write(reinterpret_cast<const char *>(&buf[0]), l);
//				os.write(reinterpret_cast<const char *>(&pad[0]), pad.size());
//			}
//		}
	}
}

#endif

