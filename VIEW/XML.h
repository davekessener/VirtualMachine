#ifndef XML_H
#define XML_H

#include <string>
#include <vector>
#include <map>
#include <iosfwd>
#include <memory>

namespace xml
{
	class Tag
	{
		typedef std::map<std::string, std::string> attr_t;
		typedef std::vector<std::shared_ptr<Tag>> tags_t;

		public:
		class Attributes
		{
			public:
			typedef attr_t::iterator iterator;
			typedef attr_t::const_iterator const_iterator;

			public:
				iterator begin( ) { return t_.attr_.begin(); }
				iterator end( ) { return t_.attr_.end(); }
				const_iterator begin( ) const { return t_.attr_.cbegin(); }
				const_iterator end( ) const { return t_.attr_.cend(); }
				const_iterator cbegin( ) const { return t_.attr_.cbegin(); }
				const_iterator cend( ) const { return t_.attr_.cend(); }
			private:
				friend class Tag;
				Attributes(Tag& t) : t_(t) { }
				Tag &t_;
		} Attributes;
		class Tags
		{
			public:
			typedef tags_t::iterator iterator;
			typedef tags_t::const_iterator const_iterator;

			public:
				iterator begin( ) { return t_.tags_.begin(); }
				iterator end( ) { return t_.tags_.end(); }
				const_iterator begin( ) const { return t_.tags_.cbegin(); }
				const_iterator end( ) const { return t_.tags_.cend(); }
				const_iterator cbegin( ) const { return t_.tags_.cbegin(); }
				const_iterator cend( ) const { return t_.tags_.cend(); }
			private:
				friend class Tag;
				Tags(Tag& t) : t_(t) { }
				Tag &t_;
		} Tags;

		public:
			explicit Tag(const std::string& name)
				: Attributes(*this), Tags(*this), name_(name) { }
			Tag( ) : Tag(".") { }
			const std::string& name( ) const { return name_; }
			bool hasTags( ) const { return !tags_.empty(); }
			std::string getAttribute(const std::string& s) const { return attr_.at(s); }
			void read(std::istream&);
		private:
			friend class Attributes;
			friend class Tags;

			std::string name_;
			attr_t attr_;
			tags_t tags_;
	};

	typedef std::shared_ptr<Tag> Tag_ptr;

	void print_xml(std::ostream&, Tag_ptr, std::string = "");

	inline std::ostream& operator<<(std::ostream& os, Tag_ptr p)
	{
		print_xml(os, p);
		return os;
	}
}

#endif

