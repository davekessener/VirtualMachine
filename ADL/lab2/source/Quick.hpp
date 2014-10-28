#ifndef DAV_QUICK_H
#define DAV_QUICK_H

#include <algorithm>
#include <type_traits>
#include <random>
#include "Sorting.hpp"

namespace dav
{
namespace sorting
{

template<typename>
struct shuffle
{
	template<typename I>
	static void run(I i1, I i2)
	{
		auto len(std::distance(i1, i2));
		std::vector<I> v(len);
		
		{
			I j(i1); for(auto i = v.begin(), e = v.end() ; i != e ; ++i, ++j) *i = j;
		}

		std::shuffle(v.begin(), v.end(), std::mt19937_64(std::random_device()()));

		std::vector<typename std::decay<decltype(*i1)>::type> buf;
		buf.reserve(len);
		for(auto& i : v) buf.push_back(*i);
		std::copy(buf.cbegin(), buf.cend(), i1);
	}
};

template<>
struct shuffle<std::random_access_iterator_tag>
{
	template<typename I>
	static void run(I i1, I i2)
	{
		std::shuffle(i1, i2, std::mt19937_64(std::random_device()()));
	}
};

template<typename I, typename T = typename std::iterator_traits<I>::iterator_category>
void do_shuffle(I i1, I i2) 
{
	shuffle<T>::run(i1, i2);
}

template<typename T, typename S = std::less<T>>
class Quick
{
	MXT_SORTING_CLASS(Quick);

	public:
		template<typename I>
		void operator()(I i1, I i2)
		{
			//do_shuffle(i1, i2);

			sort(i1, i2);
		}
	private:
		template<typename I>
		void sort(I i1, I i2)
		{
			if(islt2(i1, i2)) return;

			I i(partition(i1, i2));

			sort(i1, i);
			sort(++i, i2);
		}

		template<typename I>
		I partition(I i1, I i2)
		{
			typedef typename std::iterator_traits<I>::value_type value_type;

			I pivot(i1);
			S cmp;
			auto &e(*pivot);

			auto op = [&cmp, this](value_type& v1, value_type& v2) -> bool { MXT_SORTING_INCOP; return cmp(v1, v2); };

			if(++i1 == --i2) pivot = i2;

			while(i1 != i2)
			{
				while(!op(e, *i2)) { if(i1 == i2) break; --i2; }
				while( op(e, *i1)) { if(i1 == i2) break; ++i1; }

				if(i1 != i2) MXT_SORTING_SWAP(*i1, *i2);
			}

			if(op(e, *i2))
			{
				MXT_SORTING_SWAP(e, *i2);
				pivot = i2;
			}

			return pivot;
		}
};

}
}

#endif

