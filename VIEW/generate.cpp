#include <iostream>
#include <vector>
#include <aux>
#include "dav_header.h"

size_t get_hex(const std::string&);
void generate(std::ostream&, std::istream&, const std::vector<std::string>&);

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	generate(std::cout, std::cin, std::vector<std::string>(args.cbegin() + 1, args.cend()));

	return 0;
}

void generate(std::ostream& os, std::istream& is, const std::vector<std::string>& files)
{
	std::string pwd;
	is >> pwd;
	dav::dataheader_t head;
	head.id = dav::DAV_MAGIC;
	head.hash = dav::hash(pwd);
	head.imgcount = files.size();

	std::vector<dav::data_t> v(files.size());
	auto i = v.begin();
	for(const std::string& s : files)
	{
		i->name = get_hex(s.substr(0, 16));
		i->viewed_ms = i->viewed_c = 0;
		++i;
	}

	os << lib::aux::write_to(head);
	os << lib::aux::write_to(*v.cbegin(), v.size() * sizeof(dav::data_t));
}

size_t get_hex(const std::string& s)
{
	size_t r;
	std::stringstream ss;
	ss << s << std::setbase(16);
	ss >> r;
	return r;
}

