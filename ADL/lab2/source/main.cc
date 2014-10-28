#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <future>
#include <functional>
#include <cassert>
#include "Stack.hpp"
#include "Selection.hpp"
#include "Insertion.hpp"
#include "Merge.hpp"
#include "Quick.hpp"
#include "Timer.h"

typedef unsigned int uint;
typedef unsigned long long int_t;
typedef long double frac_t;

template<typename S>
void fill_random(S& st, int N, uint min, uint max, std::function<void(uint)> f = nullptr)
{
	static std::mt19937_64 generator(std::random_device{}());

	if(!f) f = [&st](uint i){ st.push(i); };

	std::uniform_int_distribution<uint> distribution(min, max);

	// fill stack with 2^k random numbers
	for(uint i = N ; i ; --i) f(distribution(generator));
}

template<typename S>
int_t checksort(uint k, uint min, uint max, S proto)
{
	assert(k<8*sizeof(uint));

	S sort;

	const uint N(1 << k);
	typedef dav::stack::Stack<uint> stack_t;

	stack_t s(N);
	int_t d(0);

	fill_random(s, N, min, max);

	// worst-case for merge-sort:
//	{
//		std::vector<uint> tv;
//		int j(0);
//
//		tv.reserve(N + 1);
//		tv.push_back(0);
//		while(tv.size() < N)
//		{
//			std::vector<uint> ttv(tv.size() << 1);
//			auto ii(tv.begin());
//			for(auto i(ttv.begin()), e(ttv.end()) ; i != e ; ++i)
//			{
//				*i = *ii; ++ii; ++i;
//				*i = ++j;
//			}
//			tv.swap(ttv);
//		}
//		for(auto i(tv.begin()), e(tv.end()) ; i != e ; ++i) { auto t(*i); s.push(*++i); s.push(t); }
//	}

	// sort stack (average case)
	sort(s.begin(), s.end());
	d = sort.getOPCount();
	sort.reset();

	// sort presorted stack (best case)
//	sort(s.begin(), s.end());
//	d.best = sort.getOPCount();
//	sort.reset();

	// sort presorted stack in reverse order (worst case)
//	sort(s.rbegin(), s.rend());
//	d.worst = sort.getOPCount();
//	sort.reset();

	return d;
}

template<typename I, typename S>
void do_runcheck(I i1, I i2, int k, int min, int max, S sort)
{
	while(i1 != i2)
	{
		*i1 = checksort(k, min, max, sort);
		++i1;
	}
}

template<typename S>
void runcheck(std::ostream& os, int k, int m, int min, int max, S sort)
{
	int CORES = std::thread::hardware_concurrency();
	if(!CORES) CORES = 4;

	// run 'm' sorts in 'CORES' threads
	std::vector<int_t> vec(m);
	std::vector<std::thread> mt;

	auto dorc = [k, min, max, &sort] (int_t *i1, int_t *i2)
		{
			do_runcheck(i1, i2, k, min, max, sort);
		};

	int_t *i0 = &vec.front(), *e = i0 + m;
	for(int i = CORES - 1, n(m / CORES) ; i >= 0 ; --i)
	{
		mt.push_back(std::thread(dorc, i0, i && n ? i0 + n : e));
		i0 += n;
		if(!n) break;
	}

	for(auto& f : mt)
	{
		f.join();
	}

	int_t avg(0); frac_t favg(0);
	for(const auto& d : vec)
	{
		avg += d / m;
		favg += (d % m) / (frac_t)m;
		if(favg >= 1)
		{
			avg += (int_t)favg;
			favg -= (int_t)favg;
		}
	}

	auto outp = [](int_t i, frac_t f) -> std::string
	{
		std::stringstream ss;
		ss << f;
		std::string sf(ss.str().substr(1));
		ss.clear(); ss.str("");
		ss << i;
		return ss.str() + (f < 0 ? "" : sf);
	};

	// k	best	average		worst
	os	<< k << '\t' << outp(avg, favg) << std::endl;
}

template<typename S>
void run(const std::string& s)
{
	const int MIN(2), MAX(1000000000), N(20), M(1024);

	S sort;
	dav::Timer t1, t2;
	std::ofstream out(s + ".dat");

	assert(out.is_open());

	for(int i = 1 ; i <= N ; ++i)
	{
		runcheck(out, i, M, MIN, MAX, sort);
		std::cout << "Sorted (" << s << ") " << i << " after " << t1.getDelta() << "ms" << std::endl;
	}

	out.close();

	std::cout << "Took " << t2.getDelta() << "ms for " << s << "-sort." << std::endl;
}

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	run<dav::sorting::Selection<uint>>("select");
	run<dav::sorting::Insertion<uint>>("insert");
	run<dav::sorting::Merge<uint>>("merge");
	run<dav::sorting::Quick<uint>>("quick");

	return 0;
}

