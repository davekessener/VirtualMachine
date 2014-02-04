#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct A
{
	A() { cout << "Con" << endl; }
	~A() { cout << "Des" << endl; }
	void doSomething()
	{
		cout << "Doing something." << endl;
	}
};

int main(void)
{
	A *a = new A;
	shared_ptr<A> a_p(a);
	vector<shared_ptr<A>> v = {a_p};

	vector<shared_ptr<A>>::iterator i = v.begin();

	(*i)->doSomething();

	return 0;
}

