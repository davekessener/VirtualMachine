#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <future>
#include <cassert>
#include "Stack.hpp"
#include "Selection.hpp"
#include "Insertion.hpp"
#include "Merge.hpp"
#include "Timer.h"

typedef unsigned int uint;
typedef unsigned long long int_t;
typedef long double frac_t;

struct data_t { size_t best, worst, avg; };

template<typename S>
data_t checksort(uint k, uint min, uint max, S sort)
{
	static std::mt19937_64 generator(std::random_device{}());

	assert(k<8*sizeof(uint));

	const uint N(1 << k);
	typedef dav::stack::Stack<uint> stack_t;

	std::uniform_int_distribution<uint> distribution(min, max);

	stack_t s(N);
	data_t d{0, 0, 0};

	// fill stack with 2^k random numbers
	for(uint i = N ; i ; --i) s.push(distribution(generator));

	// sort stack (average case)
	sort(s.begin(), s.end());
	d.avg = sort.getOPCount();
	sort.reset();

	// sort presorted stack (best case)
//	sort(s.begin(), s.end());
//	d.best = sort.getOPCount();
//	sort.reset();

	// sort presorted stack in reverse order (worst case)
	sort(s.rbegin(), s.rend());
	d.worst = sort.getOPCount();
	sort.reset();

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
	std::vector<data_t> vec(m);
	std::vector<std::thread> mt;

	auto dorc = [k, min, max, &sort] (data_t *i1, data_t *i2)
		{
			do_runcheck(i1, i2, k, min, max, sort);
		};

	data_t *i0 = &vec.front(), *e = i0 + m;
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

	// average out the results
	int_t   best = 0,    worst = 0,    avg = 0;
	frac_t fbest = 0.0, fworst = 0.0, favg = 0.0;
	for(const auto& d : vec)
	{
		best   += d.best  / m;
		avg    += d.avg   / m;
		worst  += d.worst / m;
		fbest  += (d.best  % m) / (frac_t)m;
		favg   += (d.avg   % m) / (frac_t)m;
		fworst += (d.worst % m) / (frac_t)m;
	}
	best  += (int_t) fbest;  fbest  -= (int_t) fbest;
	avg   += (int_t) favg;   favg   -= (int_t) favg;
	worst += (int_t) fworst; fworst -= (int_t) fworst;

	auto outp = [](int_t i, frac_t f) -> std::string
		{
			std::stringstream ss; ss << f;
			std::string frac(ss.str().substr(1));
			ss.clear(); ss.str(""); ss << i;

			return ss.str() + frac;
		};
	
	// k	best	average		worst
	os	<< k << '\t' << outp(best, fbest) << '\t' << outp(avg, favg) << '\t' << outp(worst, fworst) << std::endl;
}

template<typename S>
void run(const std::string& s)
{
	const int MIN(2), MAX(1000000000), N(20), M[] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 256, 128, 64, 32, 16, 8, 4, 4, 4, 4, 4, 4, 4} ;

	S sort;
	dav::Timer t1, t2;
	std::ofstream out(s + ".dat");

	assert(out.is_open());

	for(int i = 1 ; i <= N ; ++i)
	{
		runcheck(out, i, 1, MIN, MAX, sort);
		std::cout << "Sorted (" << s << ") " << i << " after " << t1.getDelta() << "ms" << std::endl;
	}

	out.close();

	std::cout << "Took " << t2.getDelta() << "ms for " << s << "-sort." << std::endl;
}

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);
	std::vector<std::thread> mt;

	mt.push_back(std::thread(run<dav::sorting::Merge<uint>>, std::string("merge")));
	mt.push_back(std::thread(run<dav::sorting::Selection<uint>>, std::string("select")));
	mt.push_back(std::thread(run<dav::sorting::Insertion<uint>>, std::string("insert")));
//	mt.push_back(std::thread(run<dav::sorting::MergeInplace<uint>>, std::string("inplace")));

	for(auto& t : mt)
	{
		t.join();
	}

	return 0;
}

