#include <iostream>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>

template<class T>
class A
{
	public:
		A(T&& t) : _t(*((T *) __t))
		{ 
			new((void *) __t) T(t); 
		}

		A(T& t) : _t(t) {}

		~A(void) 
		{ 
			if((void *) &_t == (void *) __t) _t.~T(); 
		}
	private:
		uint8_t __t[sizeof(T)];
		T& _t;
};

class B
{
	public:
		B(void) { std::cout << "B constructor" << std::endl; }
		B(const B& b) { std::cout << "B copy-constructor" << std::endl; }
		B(B&& b) { std::cout << "B move-constructor" << std::endl; }
		~B(void) { std::cout << "B destructor" << std::endl; }
		void doSomething(void) { std::cout << "B print" << std::endl; }
		B& operator=(const B& b) { std::cout << "B assignment operator" << std::endl; return *this; }
};

template<class T, class U>
class Conversion
{
	private:
		typedef char Small;
		struct Large { char _[2]; };
		static T _T(void);
		static Small Test(U);
		static Large Test(...);
	public:
		enum { 	exists = (sizeof(Test(_T())) == sizeof(Small)),
				same = false };
};

template<class T>
class Conversion<T, T>
{
	public:
		enum { exists = true, same = true };
};

#define INHERITS(T,U) (::Conversion<const T*,const U*>::exists&&!::Conversion<const T*,void*>::same)

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
#define NULL_t decltype(nullptr)

int main(void)
{
	std::cout << INHERITS(void *, double **) << " "
			<< INHERITS(int, double) << " "
			<< INHERITS(char, char) << std::endl;

	std::vector<std::string> c;
	std::ifstream in(std::string("test.txt"));

	while(in)
	{
		std::string s;
		getline(in, s);
		if(!s.empty()) c.push_back(s);
	}

	in.close();

	for(std::string &s : c)
	{
		std::cout << s << std::endl;
	}

	return 0;
}

