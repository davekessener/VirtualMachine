#include <iostream>
#include <fstream>
#include "Scan.h"
#include <dav/Logger.h>

#define MXT_CCEXT ".cc"

namespace
{
	bool starts_with(const std::string&, const std::string&);
	bool ends_with(const std::string&, const std::string&);
	std::string getPath(const std::string&);
	std::string simplify(const std::string&);
}

Scan::Scan(void)
{
}

int Scan::run(const params_t& args)
{
	Scan s;

	s.scan(".");

	s.generateDependencies();

	s.write(std::cout);

	return 0;
}

void Scan::scan(const path& p)
{
	using namespace boost::filesystem;

	LOG("Scanning '%s'", p.string().data());

	if(is_directory(p))
	{
		for(directory_iterator i(p), e ; i != e ; ++i)
		{
			scan(*i);
		}
	}
	else if(is_regular_file(p))
	{
		std::string fn(p.string());
		fn = fn.substr(2);

		if(ends_with(fn, MXT_CCEXT))
		{
			src_.insert(fn);
			LOG("Inserted '%s'", fn.data());
		}
	}
}

void Scan::generateDependencies(void)
{
	LOG("Generating dependencies [...]");

	for(const std::string& p : src_)
	{
		gen(p);
	}
}

void Scan::gen(const std::string& p)
{
	if(scanned_.find(p) != scanned_.cend()) return;
	scanned_.insert(p);

	LOG("Process file '%s'", p.data());

	file_list_t dep;
	std::string path(getPath(p));

	LOG("> in %s", path.data());

	std::ifstream in(p);

	while(!in.eof())
	{
		std::string line("");

		std::getline(in, line);

		if(line.empty()) continue;

		auto i = line.find_first_of('"'),
			 j = line.find_last_of('"');

		if(starts_with(line, "#include")
			&& i != j)
		{
			LOG("Found include line '%s' [%lu, %lu]", line.data(), i, j);

			std::string fn(simplify(path + line.substr(i + 1, j - i - 1)));

			LOG("> Includes %s", fn.data());
			
			dep.insert(fn);
		}
	}

	in.close();

	LOG("[DONE]");

	for(const std::string& s : dep)
	{
		gen(s);
	}

	if(!dep.empty()) deps_[p] = std::move(dep);
}

void Scan::addLib(const std::string& l)
{
	libs_.insert(l);
}

void Scan::getDepList(const std::string& p, file_list_t& list) const
{
	if(deps_.find(p) != deps_.cend())
	{
		for(const std::string& s : deps_.at(p))
		{
			if(list.find(s) == list.cend())
			{
				list.insert(s);
				getDepList(s, list);
			}
		}
	}
}

void Scan::write(std::ostream& is) const
{
	is << "CC=g++-4.7\n";
	is << "SRC=";
	for(const std::string& s : src_) is << s << " ";
	is << "\n";

	is << "OBJ=";
	for(const std::string& s : src_) is << s << ".o ";
	is << "\n";

	is << "CFLAGS=\nLIBS=\nINCDIRS=\nLIBDIRS=\n\n";

	if(boost::filesystem::is_regular_file("do.cfg"))
	{
		std::ifstream in("do.cfg");
		while(!in.eof())
		{
			std::string s("");
			std::getline(in, s);
			if(!s.empty()) is << s << "\n";
		}
		in.close();
		is << "\n";
	}

	is << "ACFLAGS=$(CFLAGS) -std=gnu++11 -Wall -O0 -ggdb -DDEBUG\n";

	is << "ALIBS=$(LIBS) ";
	for(const std::string& s : libs_) is << s << " ";
	is << "\n";

	is << "AINCDIRS=$(INCDIRS) -I/home/dave/include\n";
	is << "ALIBDIRS=$(LIBDIRS) -L/home/dave/include/lib\n\n";

	is << ".PHONY: logs clean\n\n";

	is << "all: logs $(TARGET)\n\n";

	is << "$(TARGET): $(OBJ)\n";
	is << "\t$(CC) $(OBJ) -o $(TARGET) $(ALIBDIRS) -Wl,--start-group $(ALIBS) -Wl,--end-group\n";

	for(const std::string& p : src_)
	{
		file_list_t l;

		getDepList(p, l);

		is << "\n" << p << ".o: " << p;
		for(const std::string& h : l) is << " " << h;
		is << "\n\t$(CC) -c $(ACFLAGS) $(AINCDIRS) " << p << " -o " << p << ".o\n";
	}

	is << "\nlogs:\n\tif [ -d \"logs\" ]; then for LF in *.log; "
	   << "do if [ -f \"$$LF\" ]; then mv $$LF logs/; fi; done; fi\n\n";
	is << "clean: logs\n\trm -f $(OBJ) $(TARGET)\n";

	is << std::endl;
}

namespace
{
	bool starts_with(const std::string& s, const std::string& e)
	{
		return s.length() >= e.length() && s.substr(0, e.length()) == e;
	}

	bool ends_with(const std::string& s, const std::string& e)
	{
		return s.length() >= e.length() && s.substr(s.length() - e.length()) == e;
	}

	std::string getPath(const std::string& p)
	{
		auto i = p.find_last_of('/');
		return i != std::string::npos ? p.substr(0, i) + "/" : "";
	}

	std::string simplify(const std::string& p)
	{
		auto i = p.find("..");

		if(!i) throw std::string("cannot go outside root dir");
		else if(i == std::string::npos) return p;

		auto j = p.find_last_of('/', i - 2);

		std::string s;

		if(j != std::string::npos) s = p.substr(0, j + 1);

		s += p.substr(i + 3);

		return simplify(s);
	}
}

