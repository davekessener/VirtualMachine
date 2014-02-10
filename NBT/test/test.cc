#include <iostream>
#include <initializer_list>
#include <map>
#include <cstdint>

using namespace std;

int gblVar = 0x64;

namespace A
{
	enum class Color : uint8_t
	{
		RED,
		GREEN,
		BLUE
	};

	class B
	{
		class _c
		{
			public:
				_c();
				~_c();
			private:
				friend class B;
		};

		public:
			typedef _c C;

			B();
			~B();
		private:
			C c;
	};
}

namespace A
{
	B::_c::_c(void)
	{
		cout << "Constructor C" << endl;
	}
	
	B::_c::~_c(void)
	{
		cout << "Destructor C" << endl;
	}
	
	B::B(void) : c()
	{
		cout << "Constructor B" << endl;
	}
	
	B::~B(void)
	{
		cout << "Destructor B" << endl;
	}
}

void foo(initializer_list<pair<string, int>> l)
{
	for(auto p : l)
	{
		cout << p.first << " == " << p.second << endl;
	}
}

int main(void)
{
	map<string, int> m;
	m["one"] = 1;
	m["two"] = 2;
	m["three"] = 3;

	foo(initializer_list<pair<string, int>>(m.begin(), m.end()));

	return 0;
}

