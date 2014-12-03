#include <iostream>
#include <vector>

typedef unsigned hash_t;
enum { D = 1 + 2 * 26 };

hash_t C(const char& c)
{
	if(c == ' ') return 0;
	else if(c >= 'a' && c <= 'z') return c - 'a' + 1;
	else if(c >= 'A' && c <= 'Z') return c - 'A' + ('z' - 'a' + 1) + 1;
	else throw std::string("ERR: invalid character!");
};

hash_t doHash(const std::string& s, unsigned W = 13, unsigned M = 31)
{
	hash_t v(0), t(1);

	for(auto i(s.crbegin()), e(s.crend()) ; i != e ; ++i)
	{
		v = (v + (C(*i) * t) % M) % M;
		t = (t * (D % M)) % M;
	}

	return (W * v) % M;
}

template<typename K, typename V, int M = 1000>
class HashST
{
	public:
		void put(const K&, const V&);
		V& get(const K&);
	private:
		V vals_[M];
};

template<typename K, typename V, int M>
void HashST<K, V, M>::put(const K& k, const V& v)
{
}

template<typename K, typename V, int M>
 HashST<K, V, M>::get(const K& k)
{
}

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv + 1, argv + argc);

	std::vector<std::string> test{"abc", "cba", "aaa", "a", "Az Za"};
	std::vector<int> res{21, 24, 19, 13, 5};

	std::cout << "String = precalculation = hash-function" << std::endl;
	for(unsigned i = 0 ; i < test.size() ; ++i)
	{
		std::cout << '"' << test[i] << "\" = " << res[i] << " = " << doHash(test[i]) << std::endl;
	}

	return 0;
}

