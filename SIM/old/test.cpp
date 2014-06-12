#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
	map<string, int> m;
	const string v("one");

	m[v] = 1;
	m["zwei"] = 2;
	m["tres"] = 3;

	cout << "0: " << m["one"] << endl;
	cout << "1: " << m["zwei"] << endl;
	cout << "2: " << m["tres"] << endl;

	return 0;
}

