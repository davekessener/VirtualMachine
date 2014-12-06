#include <string>
#include "GLImageBuffer.h"
#include "gl.h"

namespace dav { namespace pkmn { namespace screen {

DWORD GLImageBuffer::getImage(const void *img, size_t s)
{
	DWORD r = gl::create_texture(img, s, s, gl::RGBA);
	instance().imgs_.insert(r);
	return r;
}

DWORD GLImageBuffer::updateImage(DWORD id, const void *data, size_t s)
{
	if(!id)
	{
		return getImage(data, s);
	}
	else if(instance().imgs_.find(id) == instance().imgs_.cend())
	{
//		LOG("ERR: Trying to update invalid image, id %u.", id);
		throw std::string("trying to update invalid image!");
	}

	gl::set_texture(id, data, s, s, gl::RGBA);

	return id;
}

void GLImageBuffer::deleteImage(DWORD id)
{
	auto i = instance().imgs_.find(id);
	if(i != instance().imgs_.end())
	{
		instance().imgs_.erase(i);
		gl::delete_texture(id);
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
		gl::delete_texture(id);
	}
}

}}}

