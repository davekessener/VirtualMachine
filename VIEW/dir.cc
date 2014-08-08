#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstdlib>
#include "dir.h"
#include <boost/filesystem.hpp>

dir::dir(const std::string& file)
{
	std::string inp(file);
	if(inp.empty()) inp = "./";
	if(inp.front() == '~')
	{
		inp = inp.substr(1);
		if(!inp.empty() && inp.front() == '/') inp = inp.substr(1);
		inp = getenv("HOME") + inp;
	}
	if(inp.front() != '/' && inp.front() != '.') inp = "./" + inp;

	std::string fp(inp.substr(0, inp.find_last_of('/')));
	if(fp.empty()) fp = "/";

	boost::filesystem::path p(fp);

	std::string f;
	if(inp != "/") f = inp.substr(inp.find_last_of('/') + 1);

	for(boost::filesystem::directory_iterator i(p), e ; i != e ; ++i)
	{
		std::string fn(i->path().filename().string());
		if(fn.length() >= f.length() &&
			(f.empty() || fn.substr(0, f.length()) == f))
		{
			opt_.push_back(fn);
		}
	}

	base_ = fp;
	if(*(base_.c_str() + base_.length() - 1) != '/') base_ += "/";
	if(base_.front() == '.' && (!file.empty() && file.front() != '.'))
		base_ = base_.substr(2);

	orig_ = file;

	if(opt_.size() == 1)
	{
		best_ = opt_.front();
	}
	else if(!opt_.empty())
	{
		size_t l = -1;
		for(const std::string& s : opt_)
		{
			if(s.length() < l) l = s.length();
		}

		for(auto i = opt_.cbegin(), e = opt_.cend() ; i != e ; ++i)
		{
			for(auto j = i ; j != e ; ++j)
			{
				if(i == j) continue;
				while(i->substr(0, l) != j->substr(0, l))
				{
					if(--l == f.length()) goto opt_det_end;
				}
			}
		}
opt_det_end:

		best_ = opt_.front().substr(0, l);
	}

	opt_.push_back(f);

	cur_ = opt_.cbegin();
}

std::string dir::getNext(void)
{
	if(opt_.empty()) return "";
	auto i(cur_);
	if(++cur_ == opt_.cend()) cur_ = opt_.cbegin();
	return *i;
}

//{
//	using namespace boost::filesystem;
//
//	std::string inp(args.size() > 1 ? args.at(1) : "./");
//	if(inp.front() == '~')
//	{
//		inp = inp.substr(1);
//		if(!inp.empty() && inp.front() == '/') inp = inp.substr(1);
//		inp = getenv("HOME") + inp;
//	}
//	if(inp.front() != '/' && inp.front() != '.') inp = "./" + inp;
//	std::string fp(inp.substr(0, inp.find_last_of('/')));
//	if(fp.empty()) fp = "/";
//	path p(fp);
//	std::string f;
//	if(inp != "/") f = inp.substr(inp.find_last_of('/') + 1);
//
//	std::vector<std::string> opt;
//
//	for(directory_iterator i(p), e ; i != e ; ++i)
//	{
//		std::string fn(i->path().filename().string());
//		if(fn.length() >= f.length() && (f.empty() || fn.substr(0, f.length()) == f)) opt.push_back(fn);
//	}
//
//	std::cout << fp;
//	if(fp.back() != '/') std::cout << "/";
//
//	if(opt.size() == 1)
//	{
//		std::cout << opt.at(0);
//		if(is_directory(fp + "/" + opt.at(0))) std::cout << "/";
//	}
//	else if(!opt.empty())
//	{
//		size_t l = -1;
//		for(const std::string& s : opt)
//		{
//			if(s.length() < l) l = s.length();
//		}
//		assert(l>=f.length());
//		if(l == f.length()) std::cout << f;
//		else
//		{
//			for(auto i = opt.cbegin() ; i != opt.cend() ; ++i)
//			{
//				for(auto j = opt.cbegin() ; j != opt.cend() ; ++j)
//				{
//					if(i == j) continue;
//					while(i->substr(0, l) != j->substr(0, l))
//						if(--l == f.length()) goto end;
//				}
//			}
//end:
//			std::cout << opt.at(0).substr(0, l);
//		}
//	}
//
//	std::cout << std::endl;
//
//	return 0;
//
//}

