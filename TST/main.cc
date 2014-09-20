#include <iostream>
#include <iterator>
#include <vector>
#include <memory>
//#include <dav/stable_vector.hpp>

void pr(int i)
{
	std::cout << i << " ";
}

int main(int argc, char *argv[])
{
	pr(1), pr(2), pr(3), pr(4);

	std::cout << std::endl;
	
	return 0;

//	using namespace dav;
//
//	typedef stable_vector<std::string> vec_t;
//
//	vec_t svec;
//
//	svec.push_back("hello");
//	svec.push_back("there");
//	svec.push_back("handsome");
//
//	vec_t v2(svec);
//
//	v2.push_back("!");
//
//	std::ostream_iterator<std::string> oi(std::cout, " ");
//	std::copy(v2.cbegin(), v2.cend(), oi);
//
//	std::cout << std::endl << "Capacity: " << v2.capacity() << std::endl;
//
//	auto i = v2.begin() + 2;
//	std::cout << *i << " == ";
//	v2.erase(v2.begin() + 1);
//	v2.insert(v2.begin() + 2, "hyurgh");
//	v2.insert(v2.begin(), "first");
//	v2.insert(v2.end(), "end");
//	v2.insert(v2.begin() + 1, "second");
//	v2.erase(v2.begin() + 4);
//	std::cout << *i << std::endl;
//
//	std::cout << "Size: " << svec.size() << std::endl << "Size: " << v2.size() << std::endl;
//
//	std::copy(v2.cbegin(), v2.cend(), oi);
//
//	std::cout << std::endl;
//
//	return 0;
}

