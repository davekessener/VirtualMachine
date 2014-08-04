#include "Tags.h"

namespace dav
{
	Tag::Tag(DWORD id, const std::string& name) : id_(id), name_(name)
	{
	}

	void Tag::addChild(DWORD t)
	{
		children_.insert(t);
	}
}

