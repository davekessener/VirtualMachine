#include <iostream>
#include <memory>

using namespace std;

class A
{
	public:
		void doSomething()
		{
			cout << "Doing something (A): " << endl;
			fn();
		}
	protected:
		virtual void fn() = 0;
};

class B : public A
{
	public:
		void greet()
		{
			cout << "Hi there, handsome." << endl;
		}
	protected:
		void fn()
		{
			cout << "Having fun." << endl;
		}
};

class C : public A
{
	protected:
		void fn()
		{
			cout << "Being sad." << endl;
		}
};

typedef shared_ptr<A> A_ptr;

int main(void)
{
	A_ptr pa1(new B);
	A_ptr pa2(new C);

	pa1->doSomething();
	pa2->doSomething();

	dynamic_pointer_cast<B>(pa1)->greet();

	return 0;
}

