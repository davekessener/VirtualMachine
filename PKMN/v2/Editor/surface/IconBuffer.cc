#include "IconBuffer.h"
#include <vector>
#include <map>
#include <cassert>
#include "../../render/GLImageBuffer.h"
#include <png/png.hpp>
#include <dav/gl.h>

#define MXT_S 4
#define MXT_IS 16

namespace editor { namespace surface {

struct IconBuffer::Impl
{
	typedef std::vector<BYTE> raw_t;
	typedef std::map<std::string, DWORD> log_t;

	public:
		Impl( );
		DWORD registerIcon(const std::string&);
		void drawIcon(DWORD, int, int, int, int) const;
	private:
		DWORD id_;
		raw_t img_;
		DWORD next_;
		log_t log_;
};

// # ===========================================================================

IconBuffer::Impl::Impl(void) : img_(MXT_S * MXT_S * MXT_IS * MXT_IS * 4), next_(0)
{
	id_ = pkmn::render::GLImageBuffer::getImage(&img_.front(), MXT_S * MXT_IS);
}

DWORD IconBuffer::Impl::registerIcon(const std::string& s)
{
	if(log_.find(s) != log_.end()) return log_.at(s);

	assert(next_<MXT_S*MXT_S);
	png::image<png::rgba_pixel> img(s);

	int w = img.get_width(), h = img.get_height();

	assert(w&&w==MXT_IS&&w==h&&!(w&(w-1)));

	int x = (next_ % MXT_S) * MXT_IS, y = (next_ / MXT_S) * MXT_IS;
	for(int dy = 0 ; dy < MXT_IS ; ++dy)
	{
		for(int dx = 0 ; dx < MXT_IS ; ++dx)
		{
			auto c = img.get_pixel(dx, dy);
			img_[(x + dx + (y + dy) * MXT_S * MXT_IS) * 4 + 0] = c.red;
			img_[(x + dx + (y + dy) * MXT_S * MXT_IS) * 4 + 1] = c.green;
			img_[(x + dx + (y + dy) * MXT_S * MXT_IS) * 4 + 2] = c.blue;
			img_[(x + dx + (y + dy) * MXT_S * MXT_IS) * 4 + 3] = c.alpha;
		}
	}

	pkmn::render::GLImageBuffer::updateImage(id_, &img_.front(), MXT_S * MXT_IS);

	log_[s] = next_;

	return next_++;
}

void IconBuffer::Impl::drawIcon(DWORD id, int x1, int y1, int x2, int y2) const
{
	dav::gl::bind_texture(id_);

	float u = (id % MXT_S) / (double)MXT_S, v = (id / MXT_S) / (double)MXT_S;

	dav::gl::draw_face2d(u, v, u + 1.0/MXT_S, v + 1.0/MXT_S, x1, y1, x2, y2);
}

// # ===========================================================================

IconBuffer::IconBuffer(void) : impl_(new Impl)
{
}

IconBuffer::~IconBuffer(void)
{
	delete impl_;
}

DWORD IconBuffer::doRegisterIcon(const std::string& s)
{
	assert(instance().impl_);
	return instance().impl_->registerIcon(s);
}

void IconBuffer::doDrawIcon(DWORD id, int x1, int y1, int x2, int y2) const
{
	assert(instance().impl_);
	instance().impl_->drawIcon(id, x1, y1, x2, y2);
}

}}

