#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>

class Dir
{
	public:
	typedef std::shared_ptr<Dir> Dir_ptr;

	public:
		Dir(const std::string& s) : name_(s) { }
		Dir(const Dir& d) : name_(d.name_), cnt_(d.cnt_) { }
		Dir& operator=(const Dir& d) { Dir t(d); swap(t); return *this; }
		void addContent(Dir_ptr d) { cnt_.push_back(d); }
		bool empty( ) const { return cnt_.empty(); }
		const std::string& name( ) const { return name_; }
		bool contains(const std::string& s) const
			{ for(const Dir_ptr& p : cnt_) if(s == p->name_) return true; return false; }
		void insert(const std::string& n) { cnt_.push_back(Dir_ptr(new Dir(n))); }
		Dir_ptr get(const std::string& n)
			{ for(Dir_ptr& p : cnt_) if(p->name_ == n) return p; throw std::string("ERR: No member!"); }
		void remove(const std::string& n) { cnt_.erase(std::find(cnt_.begin(), cnt_.end(), get(n))); }
		void write(std::ostream& os, const std::string& d = "")
			{ if(empty()) os << d << name_ << "\n"; else for(Dir_ptr& p : cnt_) p->write(os, d + name_ + "/"); }
		bool operator==(const Dir&) const;
		void swap(Dir& d) noexcept { std::swap(name_, d.name_); std::swap(cnt_, d.cnt_); }
		static Dir_ptr Construct(const std::string&);
		static bool Difference(Dir_ptr, Dir_ptr);
	private:
		std::string name_;
		std::vector<Dir_ptr> cnt_;
};

typedef Dir::Dir_ptr Dir_ptr;

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	if(args.size() != 2) throw std::string("ERR: usage: " + std::string(*argv) + " file file");

	Dir_ptr d0(Dir::Construct(args.at(0)));
	Dir_ptr d1(Dir::Construct(args.at(1)));

	if(Dir::Difference(d0, d1))
	{
		std::cout << "Lists are identical." << std::endl;
	}
	else
	{
		std::ofstream out(args.at(0) + ".diff");
		d0->write(out);
		out.close();
		out.open(args.at(1) + ".diff");
		d1->write(out);
		out.close();
	}

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

bool Dir::Difference(Dir_ptr pd0, Dir_ptr pd1)
{
	if(*pd0 == *pd1) return true;

	for(Dir_ptr &p : pd0->cnt_)
	{
		if(pd1->contains(p->name_))
		{
			if(Difference(p, pd1->get(p->name_)))
			{
				pd1->remove(p->name_);
				pd0->remove(p->name_);
				return Difference(pd0, pd1);
			}
		}
	}

	return false;
}

Dir::Dir_ptr Dir::Construct(const std::string& s)
{
	std::ifstream in(s);

	if(!in || !in.is_open()) throw std::string("ERR: Couldn't open file!");

	auto split = [](const std::string& str, char d, std::vector<std::string>& v)
	{
		std::stringstream ss(str);
		std::string s;
		while(std::getline(ss, s, d)) v.push_back(s);
	};

	Dir_ptr r(new Dir("."));

	while(in && !in.eof())
	{
		std::string s("");

		std::getline(in, s);

		if(s.empty()) continue;

		std::vector<std::string> ss; split(s, '/', ss);
		Dir_ptr c(r);

		for(const std::string& n : ss)
		{
			if(n == ".") continue;
			else
			{
				if(!c->contains(n))
				{
					c->insert(n);
				}

				c = c->get(n);
			}
		}
	}

	in.close();

	return r;
}

bool Dir::operator==(const Dir& d) const
{
	if(name_ == d.name_ && cnt_.size() == d.cnt_.size())
	{
		auto isin = [](const Dir& d, const std::vector<Dir_ptr>& dds) -> bool
		{
			for(const Dir_ptr& p : dds) if(d == *p) return true; return false;
		};

		for(const Dir_ptr& p : cnt_)
		{
			if(!isin(*p, d.cnt_)) return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

