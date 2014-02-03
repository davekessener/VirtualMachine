#include <iostream>

using namespace std;

struct A
{
	void doSomething() const
	{
		do_do();
	}
	virtual void do_do() const = 0;
};

struct B : public A
{
	B(int _v) : v(_v) { }
	void do_do() const
	{
		cout << "Doing it in B(" << v << ")" << endl;
	}
	int v;
};

void exec(const A& a)
{
	a.doSomething();
}

int main(void)
{
	exec(B(3));

	return 0;
}

