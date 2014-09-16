#ifndef VIEW_TAG_H
#define VIEW_TAG_H

#include <dav/inc.h>
#include <memory>
#include <vector>
#include <string>
#include <nbt/NBT.h>

class Tag
{
	public:
	typedef std::shared_ptr<Tag> Tag_ptr;
	typedef std::vector<Tag_ptr> vec_t;

	public:
		explicit Tag(const std::string& name = "") : name_(name) { }
		explicit Tag(nbt::TAG_Compound::ptr_t);
		void insert(Tag_ptr p) { children_.push_back(p); }
		size_t size( ) const { return children_.size(); }
		bool empty( ) const { return children_.empty(); }
		template<typename I>
			void expand(I i, const std::string& t) const { findAbsolute(i, Head(t), Tail(t), ""); }
		static std::string Head(std::string&);
		static inline std::string Head(const std::string& cs) { std::string s(cs); return Head(s); }
		static inline std::string Tail(const std::string& cs) { std::string s(cs); Head(s); return s; }
		nbt::TAG_Compound::ptr_t save( ) const;
	private:
		template<typename I>
			void findAbsolute(I&&, const std::string&, const std::string&, const std::string&) const;
	private:
		std::string name_;
		vec_t children_;
};

typedef Tag::Tag_ptr Tag_ptr;

template<typename I>
void Tag::findAbsolute(I&& i, const std::string& t, const std::string& tt, const std::string& pp) const
{
	std::string path(pp + name_);
	std::string tail(tt), head(Head(tail));

	if(t.empty())
	{
		if(name_ == head)
		{
			if(tail.empty())
			{
				*i++ = path;
			}
			else for(const auto& p : children_)
			{
				p->findAbsolute(i, "", tail, path + ".");
			}
		}
	}
	else
	{
		if(name_ == t)
		{
			if(tt.empty())
			{
				*i++ = path;
			}
			else for(const auto& p : children_)
			{
				p->findAbsolute(i, "", tt, path + ".");
			}
		}
		else for(const auto& p : children_)
		{
			p->findAbsolute(i, t, tt, path + ".");
		}
	}
}

#endif

