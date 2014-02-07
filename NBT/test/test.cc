#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <cstdlib>

using namespace std;

template<class T> void foo(T& t) { cout << "lvalue" << endl; }
template<class T> void foo(T&& t) { cout << "rvalue" << endl; }

int main(void)
{
	int i;
	foo(i);
	foo(int(i));
	foo(5);

	return 0;
}

