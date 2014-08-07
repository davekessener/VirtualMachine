#include <aux>
#include "Tags.h"
#include "XML.h"

namespace dav
{
	namespace
	{
		DWORD insert_tag(TagTree&, const xml::Tag_ptr);
	}

	Tag::Tag(DWORD id, const std::string& name) : id_(id), name_(name)
	{
	}

	void Tag::addChild(DWORD t)
	{
		children_.insert(t);
	}

	void TagTree::createTag(DWORD id, const std::string& name)
	{
		if(!hasTag(id))
		{
			tags_[id] = Tag_ptr(new Tag(id, name));
		}
	}

	void TagTree::read(std::istream& is)
	{
		xml::Tag_ptr p(new xml::Tag);
		p->read(is);

		clear();

		insert_tag(*this, p);
	}

	namespace
	{
		DWORD insert_tag(TagTree& tt, const xml::Tag_ptr p)
		{
			DWORD id(lib::aux::lexical_cast<DWORD>(p->getAttribute("id")));
			tt.createTag(id, p->getAttribute("name"));
			for(const auto& pp : p->Tags)
			{
				tt.getTag(id).addChild(insert_tag(tt, pp));
			}
			return id;
		}
	}
}

