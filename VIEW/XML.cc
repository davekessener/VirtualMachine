#include <iostream>
#include <cassert>
#include "XML.h"

namespace xml
{
	namespace
	{
		bool is_in(char c, const char *s) { while(*s) if(c == *s++) return true; return false; }

		bool is_w(char c) { return is_in(c, " \t\n"); }

		char read_next(std::istream& is, std::string& s)
		{
			s = "";
			char c = '\0';
			bool q = false;

			while(!is_in(c, "</>=") || q)
			{
				if(c && (q || !is_w(c))) s.push_back(c);
				c = is.get();
				if(c == '"')
				{
					if(!q)
					{
						q = true;
						c = is.get();
					}
					else
					{
						c = is.get();
						break;
					}
				}

				if(!q && is_w(c) && !s.empty()) break;
			}

//			std::cout << "read '" << s << "' ending in '" << c << "'" << std::endl;

			return c;
		}
	}

	void Tag::read(std::istream& is)
	{
		char c;
		std::string s;

		while(true)
		{
			c = read_next(is, s);

			if(c == '<')
			{
				c = read_next(is, name_);
				assert(c!='=');
			}

			if(c == '=')
			{
				std::string v;
				c = read_next(is, v);
				if(*v.cbegin() == '"') v = std::string(v.cbegin() + 1, v.cend() - 1);
				attr_[s] = v;
			}
			
			if(c == '/')
			{
				c = read_next(is, s);
				assert(c=='>');
				return;
			}
			else if(c == '>')
			{
				break;
			}
		}

		while(true)
		{
			c = read_next(is, s);
			assert(c=='<');
			c = read_next(is, s);
			if(c == '/')
			{
				c = read_next(is, s);
				assert(c=='>');
				assert(name_==s);
				return;
			}
			
			Tag_ptr p(new Tag(s));
			p->read(is);
			tags_.push_back(p);
		}
	}

	void print_xml(std::ostream& os, Tag_ptr p, std::string pad)
	{
		bool f = false;
		
		os << pad << "<" << p->name() << " ";
		
		for(const auto& i : p->Attributes)
		{
			if(f) os << " ";
			os << i.first << "=\"" << i.second << "\"";
			f = true;
		}
		if(!p->hasTags())
		{
			os << " />\n";
		}
		else
		{
			os << ">\n";
			
			for(const auto& pp : p->Tags)
			{
				print_xml(os, pp, pad + "\t");
			}

			os << pad << "</" << p->name() << ">\n";
		}
	}
}

