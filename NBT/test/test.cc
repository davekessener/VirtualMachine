#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <cstdlib>

using namespace std;

template<function<void *(int)> make>
struct A
{
	template<typename T>
	T *create() { return reinterpret_cast<T *>(make(sizeof(T))); }
}

int main(void)
{
	A<[](int s)->void * { return std::malloc(s); }> a;

	int *i = a.create<int>();

	return 0;
}

