#include <iostream>

using namespace std;

struct A
{
	void doSomething()
	{
		do_do();
	}
	virtual void do_do() = 0;
};

struct B : public A
{
	B(int _v) : v(_v) { }
	void do_do()
	{
		cout << "Doing it in B(" << v << ")" << endl;
	}
	int v;
};

void exec(A *a)
{
	a->doSomething();
}

int main(void)
{
	B b(3);
	exec(&b);

	return 0;
}

