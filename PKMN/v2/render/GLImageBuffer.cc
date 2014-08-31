#include "GLImageBuffer.h"
#include <dav/gl.h>
#include <dav/Logger.h>

namespace pkmn { namespace render {

DWORD GLImageBuffer::getImage(const void *img, size_t s)
{
	DWORD r = dav::gl::create_texture(img, s, s, dav::gl::RGBA);
	instance().imgs_.insert(r);
	return r;
}

void GLImageBuffer::updateImage(DWORD id, const void *data, size_t s)
{
	if(!id || instance().imgs_.find(id) == instance().imgs_.cend())
	{
		LOG("ERR: Trying to update invalid image, id %u.", id);
		throw std::string("trying to update invalid image!");
	}

	dav::gl::set_texture(id, data, s, s, dav::gl::RGBA);
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

