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

template<template<typename, typename> class T>
void test(void)
{
	T<std::string, int> bst;
	TokenReader in(std::cin);

	int idx = 0;
	while(!in.empty())
	{
		std::string s(in.next());

		if(s.empty()) continue;

		if(!bst.contains(s)) bst.insert(s, idx);

		++idx;
	}

	for(auto i(bst.begin()), e(bst.end()) ; i != e ; ++i)
	{
		std::cout << i->first << ": " << i->second << std::endl;
	}

	std::cout << "\nWords: " << idx << "\nDepth: " << bst.depth() << std::endl;

	bst.clear();
}

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);
	bool rand = false;

	if(args.size() &&args.at(0) == "-r") rand = true;

	using namespace dav;

	if(rand)
	{
		test<RandomizedBinarySearchTree>();
	}
	else
	{
		test<RegularBinarySearchTree>();
	}

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

