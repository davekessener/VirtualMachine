#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "A.h"

using namespace std;

int main(void)
{
	A<3> a;
	a.addS("hello, world!");
	a.addS("hi there, handsome");
	a.addS("whatevs");

	cout << a.getAt(1) << endl;

	return 0;
}

