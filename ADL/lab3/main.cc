#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include "PQueue.hpp"
#include "MeritOrder.hpp"

typedef std::pair<std::string, double> Pair;

template<typename T>
struct ident
{
	typedef typename std::decay<T>::type type;

	const type& operator()(const type& t) { return t; }
};

#define MXT_DEF_GET(name) \
template<typename T> \
struct get_pair_##name \
{ \
	typedef typename T::name##_type type; \
	const type& operator()(const T& t) { return t.name; } \
};

MXT_DEF_GET(first);
MXT_DEF_GET(second);

#undef MXT_DEF_GET

template<typename T, typename E = ident<T>, typename S = std::less<typename E::type>>
struct cmp
{
	bool operator()(const T& v1, const T& v2) const
	{
		return S()(E()(v1), E()(v2));
	}
};

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	dav::PQueue<Pair, cmp<Pair, get_pair_second<Pair>>> pq;
	std::vector<int> v;
	auto fn = [](int i) -> Pair { char c[] = {'a', '\0'}; c[0] += i; return Pair(c, i); };

	for(int i = 0 ; i < 26 ; ++i) v.push_back(i);
	std::random_shuffle(v.begin(), v.end());
	for(const auto& i : v) { std::cout << (char)(i + 'a') << ' '; pq.insert(fn(i)); } std::cout << std::endl;

	while(!pq.empty()) { std::cout << pq.top().first << ' '; pq.remove(); } std::cout << std::endl;

	std::vector<dav::mo> mov;
	while(true)
	{
		std::string s;
		std::getline(std::cin, s);
		if(!std::cin || std::cin.eof()) break;
		if(s.empty()) continue;
		std::istringstream iss(s);
		dav::mo mo;
		iss >> mo.first;
		iss >> mo.second;
		mov.push_back(mo);
	}

	struct wrapper
	{
		wrapper& operator*() { return *this; }
		wrapper& operator=(const dav::mo& mo) { std::cout << mo.first << '\t' << mo.second << '\n'; return *this; }
		wrapper& operator++( ) { return *this; }
		wrapper& operator++(int) { return *this; }
	};

	dav::merit_order(mov.cbegin(), mov.cend(), wrapper());

	return 0;
}

