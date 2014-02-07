#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <cstdlib>
#include <cassert>

using namespace std;

void out() {}

template<typename T, typename ... R>
void out(T t, R ... r)
{
	cout << t << endl;
	out(r...);
}

int main(void)
{
	out("This is a % == %, now a percentage sign %% and pi == %\n", string("three"), 3, 3.14156);

	return 0;
}

