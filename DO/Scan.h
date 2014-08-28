#ifndef DO_SCAN_H
#define DO_SCAN_H

#include <iosfwd>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <boost/filesystem.hpp>

typedef std::vector<std::string> params_t;

class Scan
{
	typedef boost::filesystem::path path;
	typedef std::set<std::string> file_list_t;
	typedef std::map<std::string, file_list_t> dep_map_t;

	public:
		Scan( );
		static int run(const params_t&);
		void scan(const path&);
		void generateDependencies( );
		void getDepList(const std::string&, file_list_t&) const;
		void gen(const std::string&);
		void addLib(const std::string&);
		void write(std::ostream&) const;
	private:
		file_list_t src_;
		dep_map_t deps_;
		file_list_t libs_;
		file_list_t scanned_;
};

#endif

