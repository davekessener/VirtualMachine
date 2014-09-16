#include "Tag.h"
#include <dav/Logger.h>

Tag::Tag(nbt::TAG_Compound::ptr_t tag) : name_(tag->getName())
{
	for(auto i = tag->Tags.begin(), e = tag->Tags.end() ; i != e ; ++i)
	{
		children_.push_back(Tag_ptr(new Tag(std::dynamic_pointer_cast<nbt::TAG_Compound>(*i))));
	}
}

std::string Tag::Head(std::string& s)
{
	std::string r;

	if(s.empty()) return "";

	if(s.at(s.length() - 1) == '.')
	{
		LOG("ERR: Path '%s' cannot end in '.'", s.data());
		throw std::string("path cannot end in '.'!");
	}

	size_t i = s.find_first_of('.');

	if(i != std::string::npos)
	{
		if(i)
		{
			r = s.substr(0, i);
		}

		s = s.substr(i + 1);
	}
	else
	{
		r = s;
		s = "";
	}

	return r;
}

nbt::TAG_Compound::ptr_t Tag::save(void) const
{
	nbt::TAG_Compound::ptr_t tag = nbt::Make<nbt::TAG_Compound>(name_);

	for(const auto& p : children_)
	{
		tag->setTag(p->save());
	}

	return tag;
}

