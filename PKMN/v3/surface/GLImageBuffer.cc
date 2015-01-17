#include <string>
#include "GLImageBuffer.h"
#include <dav/gl.h>

namespace dav { namespace pkmn { namespace screen {

DWORD GLImageBuffer::GetImage(const void *img, size_t s)
{
	DWORD r = gl::create_texture(img, s, s, gl::RGBA);
	Instance().imgs_.insert(r);
	return r;
}

DWORD GLImageBuffer::UpdateImage(DWORD id, const void *data, size_t s)
{
	if(!id)
	{
		return GetImage(data, s);
	}
	else if(Instance().imgs_.find(id) == Instance().imgs_.cend())
	{
//		LOG("ERR: Trying to update invalid image, id %u.", id);
		throw std::string("trying to update invalid image!");
	}

	gl::set_texture(id, data, s, s, gl::RGBA);

	return id;
}

void GLImageBuffer::SetNear(DWORD id)
{
	auto i = Instance().imgs_.find(id);
	if(i != Instance().imgs_.end())
	{
		gl::mod_texture(id, gl::NEAR);
	}
}

void GLImageBuffer::SetLinear(DWORD id)
{
	auto i = Instance().imgs_.find(id);
	if(i != Instance().imgs_.end())
	{
		gl::mod_texture(id, gl::LINEAR);
	}
}

void GLImageBuffer::DeleteImage(DWORD id)
{
	auto i = Instance().imgs_.find(id);
	if(i != Instance().imgs_.end())
	{
		Instance().imgs_.erase(i);
		gl::delete_texture(id);
	}
}

GLImageBuffer& GLImageBuffer::Instance(void)
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

