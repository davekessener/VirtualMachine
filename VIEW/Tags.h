#ifndef TAGS_H
#define TAGS_H

#include <string>
#include <set>
#include <map>
#include <memory>
#include <algorithm>
#include "inc.h"

namespace dav
{
	class Tag
	{
		public:
			Tag(DWORD, const std::string&);
			void addChild(DWORD);
			inline DWORD id( ) const { return id_; }
			inline const std::string& name( ) const { return name_; }
		private:
			DWORD id_;
			std::string name_;
			std::set<DWORD> children_;
	};

	typedef std::shared_ptr<Tag> Tag_ptr;

	class TagTree
	{
		public:
			bool hasTag(DWORD id) const { return tags_.find(id) != tags_.cend(); }
			void createTag(DWORD, const std::string&);
			Tag& getTag(DWORD id) { return *tags_.at(id); }
		private:
			std::map<DWORD, Tag_ptr> tags_;
	};
}

#endif

