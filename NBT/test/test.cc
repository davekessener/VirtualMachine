#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <cstdlib>

using namespace std;

struct A
{
	template<typename T>
	A(T& t) : l(t.size()) { }
	template<typename T>
	A(T&& t) : l(t.size()) { }
	int l;
};

template<class T> void foo(T& t) { }
template<class T> void foo(T&& t) { }

int main(void)
{
	int i;
	foo(i);

	return 0;
}

