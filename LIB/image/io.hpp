#ifndef LIB_IMAGE_IO_H
#define LIB_IMAGE_IO_H

#include <iosfwd>
#include <iterator>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>
#include "headers.h"
#include "aux.hpp"

namespace lib
{
	namespace img
	{
		template<typename DIB, typename I, typename D>
		struct BMP
		{
			typedef typename aux::deref_trait<I>::type color_type;
			typedef D data_type;

			bmp_header_t bmp_h;
			DIB dib_h;
			I iter;
		};

		struct dib_holder
		{
			std::uint32_t id;
			int width, height;
			unsigned int bpp;
			std::vector<std::uint8_t> data;
			std::shared_ptr<std::vector<std::uint8_t>> raw;
		};
		typedef BMP<dib_holder, std::uint8_t *, std::uint8_t> bmp_reader;

		template<typename DIB>
		void read_dib(bmp_reader&, std::istream&)
		{
			assert(!"Shouldn't be here!");
		}

		template<>
		void read_dib<dib_BITMAPINFOHEADER_t>(bmp_reader& bmp, std::istream& is)
		{
			typedef std::vector<std::uint8_t> buf_t;
			typedef std::shared_ptr<buf_t> buf_ptr;

			dib_BITMAPINFOHEADER_t dib;
			dib.headersize = bmp.dib_h.id;
			std::copy_n(bmp.dib_h.data.begin(), dib.headersize, reinterpret_cast<std::uint8_t *>(&dib) + sizeof(dib.headersize));
			bmp.dib_h.width = dib.width;
			bmp.dib_h.height = dib.height;
			bmp.dib_h.bpp = dib.bpp;

			assert(dib.width>0&&dib.height>0);
			
			unsigned int rowsize = ((dib.bpp * dib.width + 31) / 32) * 4;

			buf_t buf(rowsize);
			buf_ptr data(new buf_t);

			for(int h = 0 ; h < dib.height ; ++h)
			{
				is >> aux::read_from(*buf.begin(), buf.size());
				data->insert(data->end(), buf.begin(), buf.end());
			}

			assert(data->size()==static_cast<unsigned>(dib.bpp/8*dib.width*dib.height));

			bmp.dib_h.raw = data;
			bmp.iter = &*data->begin();
		}

		inline bmp_reader read_bmp(std::istream& is)
		{
			bmp_reader bmp;
			is >> aux::read_from(bmp.bmp_h);
			is >> bmp.dib_h.id;
			bmp.dib_h.data.resize(bmp.dib_h.id);
			is >> aux::read_from(*bmp.dib_h.data.begin(), bmp.dib_h.id);

			switch(bmp.dib_h.id)
			{
				case sizeof(dib_BITMAPINFOHEADER_t):
					read_dib<dib_BITMAPINFOHEADER_t>(bmp, is);
					break;
				default:
					assert(!"ERR: unknown size!");
			}

			return bmp;
		}

		template<typename DIB, typename I, typename D>
		BMP<DIB, I, D> populate_bmp(const DIB& dib, I i)
		{
			BMP<DIB, I, D> bmp;

			bmp.dib_h = dib;
			bmp.dib_h.headersize = sizeof(DIB);
			bmp.dib_h.planes = 1;
			bmp.dib_h.bpp = sizeof(D) * 8;
			bmp.dib_h.palettecount = 0;
			bmp.dib_h.importantcolors = 0;

			unsigned int rowsize = ((dib.bpp * dib.width + 31) / 32) * 4;

			bmp.bmp_h.id = BMP_MAGIC;
			bmp.bmp_h.reserved = 0;
			bmp.bmp_h.offset = sizeof(bmp_header_t) + sizeof(DIB);
			bmp.bmp_h.filesize = bmp.bmp_h.offset + rowsize * dib.height;

			bmp.iter = i;

			return bmp;
		}

		template<typename DIB, typename Image>
		inline auto create_bmp(const DIB& dib, Image& i)
			-> BMP<DIB, typename Image::iterator, typename Image::data_type>
		{
			return populate_bmp<DIB, typename Image::iterator, typename Image::data_type>(dib, i.begin());
		}

		template<typename DIB, typename Image>
		inline auto create_bmp(const DIB& dib, const Image& i)
			-> BMP<DIB, typename Image::const_iterator, typename Image::data_type>
		{
			return populate_bmp<DIB, typename Image::const_iterator, typename Image::data_type>(dib, i.cbegin());
		}

		template<typename DIB, typename I, typename D>
		void write_bmp(const BMP<DIB, I, D>& bmp, std::ostream& os)
		{
			typedef typename BMP<DIB, I, D>::color_type C;

			unsigned int l = bmp.dib_h.width * (bmp.dib_h.bpp / 8);
			unsigned int rowsize = ((bmp.dib_h.bpp * bmp.dib_h.width + 31) / 32) * 4;

			std::vector<D> buf(bmp.dib_h.width);
			std::vector<std::uint8_t> pad(rowsize - l);
			I i(bmp.iter);

			os << aux::write_to(bmp.bmp_h);
			os << aux::write_to(bmp.dib_h);

			for(int h = 0 ; h < bmp.dib_h.height ; ++h)
			{
				aux::transform_n(i, bmp.dib_h.width, buf.begin(), [](const C& c) { return static_cast<D>(c); });
				os << aux::write_to(*buf.begin(), l);
				os << aux::write_to(*pad.begin(), pad.size());
			}
		}

		template<typename DIB, typename I, typename D>
		std::ostream& operator<<(std::ostream& os, const BMP<DIB, I, D>& bmp)
		{
			write_bmp(bmp, os);
			return os;
		}
	}
}

#endif

