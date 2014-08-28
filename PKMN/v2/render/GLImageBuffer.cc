#include "GLImageBuffer.h"
#include <dav/gl.h>

namespace pkmn { namespace render {

DWORD GLImageBuffer::getImage(const void *img, size_t s)
{
	DWORD r = dav::gl::create_texture(img, s, s, dav::gl::RGBA);
	instance().imgs_.insert(r);
	return r;
}

void GLImageBuffer::deleteImage(DWORD id)
{
	auto i = instance().imgs_.find(id);
	if(i != instance().imgs_.end())
	{
		instance().imgs_.erase(i);
		dav::gl::delete_texture(id);
	}
}

GLImageBuffer& GLImageBuffer::instance(void)
{
	static GLImageBuffer gli;
	return gli;
}

GLImageBuffer::~GLImageBuffer(void) noexcept
{
	for(const DWORD& id : imgs_)
	{
		dav::gl::delete_texture(id);
	}
}

}}

