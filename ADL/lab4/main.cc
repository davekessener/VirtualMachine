#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <random>
#include <functional>
#include "BST.hpp"

class TokenReader
{
	public:
		TokenReader(std::istream&);
		std::string next( );
		bool empty( ) const { return v_.empty(); }
	private:
		std::deque<std::string> v_;
};

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	using namespace dav;

	BinarySearchTree<std::string, int> bst;
//	std::map<std::string, int> bst;
	TokenReader in(std::cin);

	int idx = 0;
	while(!in.empty())
	{
		std::string s(in.next());

		if(s.empty()) continue;

		if(!bst.contains(s)) bst.insert(s, idx);
//		if(!bst.count(s)) bst[s] = idx;

		++idx;
	}

	for(auto i(bst.cbegin()), e(bst.cend()) ; i != e ; ++i)
	{
		std::cout << i->first << ": " << i->second << std::endl;
	}

	std::cout << "\nWords: " << idx << "\nDepth: " << bst.depth() << std::endl;

	bst.clear();

	BinarySearchTree<size_t, int> tree;
	std::mt19937_64 gen(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, static_cast<size_t>(-1));
	idx = 0;
	for(size_t n = 1 ; n <= 20 ; ++n)
	{
		for(size_t i = 0, e = idx + 1 ; i < e ; ++i)
		{
			tree.insert(dist(gen), 0); ++idx;
		}

		std::cout << "\nSize: 2^" << n << "-1=" << idx << "\nDepth: " << tree.depth() << std::endl;
	}

	std::cout << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

TokenReader::TokenReader(std::istream& in)
{
	std::stringstream ss;
	
	while(in && !in.eof())
	{
		std::string s("");
		
		std::getline(in, s);
		
		ss << s << ' ';
	}

	std::string str(ss.str());

	std::string s("");
	for(const auto& c : str)
	{
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '\'')
		{
			s.push_back(c);
		}
		else
		{
			if(!s.empty()) { v_.push_back(s); s = ""; }
		}
	}
}

std::string TokenReader::next(void)
{
	if(empty()) throw std::string("empty stream");

	std::string s(v_.front());
	v_.pop_front();

	return s;
}

