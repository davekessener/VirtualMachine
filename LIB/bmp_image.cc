#include <cassert>
#include "bmp_image.h"
#include "bmp_header.h"

namespace lib { namespace img {

struct bmp_image::Impl
{
	Impl(std::istream&);
	Impl(std::uint8_t *, size_t, size_t, ColorType);
	void toFile(std::ostream&) const;

	std::uint8_t *data_;
	size_t w_, h_;
	ColorType ct_;
};

// # ===========================================================================

bmp_image::Impl::Impl(std::istream& is)
{
	bmp_header_t bmp_h;
	dib_header_t dib_h;

	is.read(reinterpret_cast<char *>(&bmp_h), sizeof(bmp_h));
	
	assert(bmp_h.id==BMP_MAGIC);

	is >> dib_h.headersize;
}

bmp_image::Impl::Impl(std::uint8_t *d, size_t w, size_t h, ColorType ct)
	: data_(d), w_(w), h_(h), ct_(ct)
{
	if(!data_)
	{
		data_ = new std::uint8_t[w * h * static_cast<size_t>(ct_)];
	}
}

void bmp_image::Impl::toFile(std::ostream& os) const
{
}

// # ===========================================================================

bmp_image::bmp_image(std::istream& is) : impl_(new Impl(is))
{
}

bmp_image::bmp_image(std::uint8_t *d, size_t w, size_t h, ColorType ct)
	: impl_(new Impl(d, w, h, ct))
{
}

void bmp_image::toFile(std::ostream& os) const
{
	impl_->toFile(os);
}

std::uint8_t *bmp_image::getRawData(void)
{
	return impl_->data_;
}

const std::uint8_t *bmp_image::getRawData(void) const
{
	return impl_->data_;
}

ColorType bmp_image::getColorType(void) const
{
	return impl_->ct_;
}

size_t bmp_image::getWidth(void) const
{
	return impl_->w_;
}

size_t bmp_image::getHeight(void) const
{
	return impl_->h_;
}

// # ===========================================================================

bmp_image::bmp_image(void) : impl_(NULL)
{
}

bmp_image::bmp_image(const bmp_image& img) : impl_(img.impl_ ? new Impl(*img.impl_) : NULL)
{
}

bmp_image::bmp_image(bmp_image&& img) : impl_(NULL)
{
	swap(img);
}

bmp_image::~bmp_image(void)
{
	delete impl_;
}

bmp_image& bmp_image::operator=(const bmp_image& img)
{
	bmp_image t(img);
	swap(t);
	return *this;
}

bmp_image& bmp_image::operator=(bmp_image&& img)
{
	swap(img);
	return *this;
}

void bmp_image::swap(bmp_image& img) noexcept
{
	Impl *i = impl_;
	impl_ = img.impl_;
	img.impl_ = i;
}

}}

