#include <iostream>
#include <vector>
#include "Manager.h"
#include "Tag.h"
#include <dav/Logger.h>

class Stringifier
{
	public:
		explicit Stringifier(const std::string& s = "") : sep_(s), v_(new std::vector<std::string>) { }
		Stringifier& operator*( ) { return *this; }
		Stringifier& operator=(const std::string& s) { LOG("Pushed back '%s'", s.data()); v_->push_back(s); return *this; }
		Stringifier& operator++( ) { return *this; }
		Stringifier& operator++(int) { return *this; }
		void print(std::ostream&) const;
	private:
		std::string sep_;
		std::shared_ptr<std::vector<std::string>> v_;
};

inline std::ostream& operator<<(std::ostream& os, const Stringifier& ss)
{
	ss.print(os);
	return os;
}

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

//	Tag_ptr root(new Tag);
//	Tag_ptr A(new Tag("A")), B(new Tag("B")), C(new Tag("C"));
//	Tag_ptr a(new Tag("a")), b(new Tag("b"));
//
//	A->insert(a);
//	A->insert(b);
//	B->insert(b);
//
//	root->insert(A);
//	root->insert(B);
//	root->insert(C);
//
//	Stringifier s("|");
//	root->expand(s, "b");
//
//	std::cout << "(" << s << ")" << std::endl;
//
//	nbt::writeFile("tags.nbt", root->save());
//
//	return 0;

	return Manager::instance().run(args);
}

void Stringifier::print(std::ostream& os) const
{
	bool f(false), ff(!sep_.empty());

	for(const std::string& s : *v_)
	{
		if(ff && f) os << sep_;
		os << s;
		f = true;
	}
}

