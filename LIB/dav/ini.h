#ifndef DAV_INI_H
#define DAV_INI_H

#include <string>
#include <iosfwd>
#include <map>
#include <aux>

namespace dav
{
	class INI
	{
		typedef std::map<std::string, std::string> topics_t;
		typedef std::map<std::string, topics_t> map_t;

		public:
			INI( ) { }
			INI(std::istream& is) { read(is); }
			bool hasTopic(const std::string& s) const { return map_.find(s) != map_.cend(); }
			void addTopic(const std::string&, bool = false);
			void eraseTopic(const std::string& s) { if(hasTopic(s)) map_.erase(map_.find(s)); }
			void selectTopic(const std::string& s) { if(hasTopic(s)) cur_ = s; }
			const std::string& getCurrentTopic( ) const { return cur_; }
			void addTag(const std::string& s) { addTag(cur_, s, ""); }
			void addTag(const std::string& s, const std::string& c) { addTag(cur_, s, c); }
			void addTag(const std::string& t, const std::string& s, const std::string& c) { map_.at(t)[s] = c; }
			const std::string& getTag(const std::string& s) const { return map_.at(cur_).at(s); }
			void eraseTag(const std::string& s) { eraseTag(cur_, s); }
			void eraseTag(const std::string&, const std::string&);
			std::string& operator[](const std::string& s) { return map_.at(cur_)[s]; }
			size_t size( ) const;
			size_t topicSize( ) const { return map_.at(cur_).size(); }
			void setString(const std::string& s, const std::string& v) { addTag(s, v); }
			void setInt(const std::string& s, long v) { addTag(s, aux::lexical_cast<std::string>(v)); }
			void setDouble(const std::string& s, double v) { addTag(s, aux::lexical_cast<std::string>(v)); }
			std::string getString(const std::string& s) const { return getTag(s); }
			long getInt(const std::string& s) const { return aux::lexical_cast<long>(getTag(s)); }
			double getDouble(const std::string& s) const { return aux::lexical_cast<double>(getTag(s)); }
			std::string getString(const std::string&, const std::string&);
			long getInt(const std::string&, long);
			double getDouble(const std::string&, double);
			void read(std::istream&);
			void write(std::ostream&) const;
		private:
			map_t map_;
			std::string cur_;
	};

	inline std::istream& operator>>(std::istream& is, INI& ini)
	{
		ini.read(is);
		return is;
	}

	inline std::ostream& operator<<(std::ostream& os, const INI& ini)
	{
		ini.write(os);
		return os;
	}
}

#endif

