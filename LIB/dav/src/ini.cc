#include <iostream>
#include "ini.h"

namespace dav
{
	void INI::addTopic(const std::string& t, bool select)
	{
		if(!hasTopic(t))
		{
			map_.insert(std::make_pair(t, topics_t()));
		}

		if(select)
		{
			cur_ = t;
		}
	}

	void INI::eraseTag(const std::string& t, const std::string& s)
	{
		auto &tbl(map_.at(t));
		auto i = tbl.find(s);

		if(i != tbl.end()) tbl.erase(i);
	}

	size_t INI::size(void) const
	{
		size_t s = map_.size();

		for(const auto& p : map_)
		{
			s += p.second.size();
		}

		return s;
	}

	std::string INI::getString(const std::string& s, const std::string& d)
	{
		auto &c(map_.at(cur_));
		auto i = c.find(s);

		return i != c.end() ? i->second : c[s] = d;
	}

	long INI::getInt(const std::string& s, long d)
	{
		using aux::lexical_cast;

		auto &c(map_.at(cur_));
		auto i = c.find(s);

		return i != c.end() ? lexical_cast<long>(i->second)
							: (c[s] = lexical_cast<std::string>(d), d);
	}

	double INI::getDouble(const std::string& s, double d)
	{
		using aux::lexical_cast;

		auto &c(map_.at(cur_));
		auto i = c.find(s);

		return i != c.end() ? lexical_cast<double>(i->second)
							: (c[s] = lexical_cast<std::string>(d), d);
	}

	void INI::read(std::istream& is)
	{
		map_t().swap(map_);

		while(!is.eof())
		{
			std::string line("");

			std::getline(is, line);

			if(line.empty()) continue;

			if(line.front() == '[')
			{
				addTopic(line.substr(1, line.length() - 2), true);
			}
			else
			{
				size_t p = line.find_first_of('=');

				addTag(cur_, line.substr(0, p), line.substr(p + 1));
			}
		}
	}

	void INI::write(std::ostream& os) const
	{
		for(const auto& t : map_)
		{
			os << "[" << t.first << "]\n";
			for(const auto& p : t.second)
			{
				os << p.first << "=" << p.second << "\n";
			}
			os << "\n";
		}
	}
}

