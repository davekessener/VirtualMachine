#include <vector>
#include <cassert>
#include "GLBuffer.h"
#include <dav/gl.h>

using namespace dav;

GLBuffer::GLBuffer(void) : id_(0), w_(0), h_(0), pp_(0)
{
}

GLBuffer::~GLBuffer(void) noexcept
{
	for(const auto& i : tex_)
	{
		gl::delete_texture(i.second);
	}
}

void GLBuffer::set(const BYTE *data, int width, int height)
{
	w_ = width;
	h_ = height;

	pp_ = 1;
	while(pp_ < w_ || pp_ < h_) pp_ <<= 1;
	rw_ = w_ / (float)pp_;
	rh_ = h_ / (float)pp_;

	std::vector<BYTE> buf(pp_ * pp_ * 3);
	for(int y = 0 ; y < h_ ; ++y)
	{
		for(int x = 0 ; x < w_ ; ++x)
		{
			buf[x * 3 + 0 + y * pp_ * 3] = data[x * 3 + 0 + y * w_ * 3];
			buf[x * 3 + 1 + y * pp_ * 3] = data[x * 3 + 1 + y * w_ * 3];
			buf[x * 3 + 2 + y * pp_ * 3] = data[x * 3 + 2 + y * w_ * 3];
		}
	}

	if(!hasTexture(pp_))
	{
		id_ = gl::create_texture(&*buf.cbegin(), pp_, pp_, gl::RGB, gl::LINEAR);

		tex_[pp_] = id_;
	}
	else
	{
		id_ = tex_[pp_];
		
		gl::set_texture(id_, &*buf.cbegin(), pp_, pp_);
	}
}

void GLBuffer::render(float u1, float v1, float u2, float v2, int x1, int y1, int x2, int y2) const
{
	gl::draw_face2d(u1 * rw_, v1 * rh_, u2 * rw_, v2 * rh_, x1, y1, x2, y2);
}

void GLBuffer::bind(void) const
{
	gl::bind_texture(id_);
}

bool GLBuffer::hasTexture(int p) const
{
	return tex_.find(p) != tex_.cend();
}

