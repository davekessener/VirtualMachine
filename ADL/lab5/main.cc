#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>

class TokenReader
{
	public:
		TokenReader(std::istream&);
		std::string next( );
		bool empty( ) const { return v_.empty(); }
	private:
		std::deque<std::string> v_;
};


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

template<typename K, typename V>
class HashST
{
	typedef std::pair<const K, V> type;
	typedef type *pointer;

	public:
		HashST(int c) : M(c), cnt_(0), vals_(new pointer[M]) { for(pointer *p = vals_, *e = p + M ; p != e ; ++p) *p = nullptr; }
		~HashST( ) { delete[] vals_; }
		void put(const K&, const V&);
		V& get(const K&);
		bool contains(const K& k) { return find(k); }
		int count( ) const { return cnt_; }
	private:
		pointer find(const K&);
	public:
		const int M;
	private:
		unsigned cnt_;
		pointer *vals_;
};

template<typename K, typename V, int M>
void HashST<K, V, M>::put(const K& k, const V& v)
{
	hash_t i(doHash(k) % M);
	cnt_ = 1;

	while(vals_[i] != nullptr)
	{
		++cnt_;
		if(vals_[i]->first == k) break;
		if(++i == M) i = 0;
	}

	if(!vals_[i]) vals_[i] = new type(k, v);
	else vals_[i]->second = v;
}

template<typename K, typename V, int M>
typename HashST<K, V, M>::pointer HashST<K, V, M>::find(const K& k)
{
	hash_t i(doHash(k) % M);
	cnt_ = 1;

	while(vals_[i] && ++cnt_ && vals_[i]->first != k) if(++i == M) i = 0;

	return vals_[i];
}

template<typename K, typename V, int M>
V& HashST<K, V, M>::get(const K& k)
{
	pointer p(find(k));

	if(!p) throw std::string("ERR: Unknown key: " + k);

	return p->second;
}

template<typename I>
void checkHashST(I i1, I i2, size_t M)
{
	HashST map(M);

	int c(0), i(0);
	while(i1 != i2)
	{
		map.put(i1->first, i1->second);
		c += map.count;
		++i;
	}

	std::cout << "
}

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	std::vector<std::string> test{"abc", "cba", "aaa", "a", "Az Za", "A Long Hashing Example"};
	std::vector<int> res{21, 24, 19, 13, 5, -1};

	std::cout << "String = precalculation = hash-function" << std::endl;
	for(unsigned i = 0 ; i < test.size() ; ++i)
	{
		std::cout << '"' << test[i] << "\" = " << res[i] << " = " << doHash(test[i]) << std::endl;
	}

	std::ifstream in("text.txt");
	TokenReader reader(in);

	typedef pair_t std::pair<std::string, int>;
	std::vector<pair_t> words;
	
	{
		std::set<std::string> table;

		for(int i = 0 ; !reader.empty() ; ++i)
		{
			std::string s(reader.next());

			if(!table.contains(s))
			{
				table.insert(s);
				words.push_back(std::make_pair(s, i));
			}
		}
	}

	checkHashST(words.cbegin(), words.cend(), words.size() * 4 + 1);
	checkHashST(words.cbegin(), words.cend(), words.size() * 2 + 1);
	checkHashST(words.cbegin(), words.cend(), words.size() + 1);

	in.close();

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

TokenReader::TokenReader(std::istream& in)
{
	std::stringstream ss;
	
	while(in && !in.eof())
	{
		std::string s("");
		
		std::getline(in, s);
		
		ss << s << ' ';
	}

	std::string str(ss.str());

	std::string s("");
	for(const auto& c : str)
	{
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			s.push_back(c);
		}
		else
		{
			if(!s.empty()) { v_.push_back(s); s = ""; }
		}
	}
}

std::string TokenReader::next(void)
{
	if(empty()) throw std::string("empty stream");

	std::string s(v_.front());
	v_.pop_front();

	return s;
}


