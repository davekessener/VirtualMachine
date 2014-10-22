#ifndef DAV_MERGE_H
#define DAV_MERGE_H

#include "Sorting.hpp"
#include <type_traits>

namespace dav
{
	namespace sorting
	{
		struct merge_simple { };
		struct merge_inplace { };

		template<typename T, typename S, typename TT>
		class MergeImpl;

		template<typename T, typename S>
		class MergeImpl<T, S, merge_simple>
		{
			MXT_SORTING_CLASS(MergeImpl);

			public:
				template<typename I>
				void operator()(I i1, I i2)
				{
					std::vector<typename std::decay<decltype(*i1)>::type> buf(std::distance(i1, i2));

					sort(i1, i2, buf.begin());
				}
			private:
				template<typename I1, typename I2>
				void sort(I1 begin, I1 end, I2 buf)
				{
					auto d(std::distance(begin, end));
					if(d < 2) return;

					I1 i(begin); advance(i, d / 2);

					sort(begin, i, buf);
					sort(i, end, buf);
					merge(begin, i, end, buf);
				}

				template<typename I1, typename I2>
				void merge(I1 begin, I1 mid, I1 end, I2 buf)
				{
					I1 i(begin), j(mid);
					I2 k(buf);
					S op;

					while(i != mid || j != end)
					{
						if(i == mid)
						{
							while(j != end) { *k = *j; ++j; ++k; MXT_SORTING_INCOP; } break;
						}
						else if(j == end)
						{
							while(i != mid) { *k = *i; ++i; ++k; MXT_SORTING_INCOP; } break;
						}
						else
						{
							if(!op(*i, *j)) *k = *i++;
							else *k = *j++;
							MXT_SORTING_INCOP;
						}

						++k;
						MXT_SORTING_INCOP;
					}

					MXT_SORTING_COPY(buf, k, begin);
				}
		};

		template<typename T, typename S>
		class MergeImpl<T, S, merge_inplace>
		{
			MXT_SORTING_CLASS(MergeImpl);

			public:
				template<typename I>
				void operator()(I i1, I i2)
				{
					auto d(std::distance(i1, i2));

					if(d < 2) return;

					I i(i1); std::advance(i, d / 2);

					(*this)(i1, i);
					(*this)(i, i2);
					merge(i1, i, i2);
				}
			private:
				template<typename I>
				void merge(I begin, I mid, I end)
				{
					typedef std::reverse_iterator<I> riter;
					I i(begin), j(mid);
					S op;

					while(i != j && i != end && j != end)
					{
						if(op(*i, *j))
						{
							auto t(*j);
							I e(j); ++e;
							
							MXT_SORTING_COPY(riter(j), riter(i), riter(e));

							*i = t;
							++j;
						}

						++i;
						MXT_SORTING_INCOP;
					}
				}
		};

		template<typename T, typename S = std::less<T>>
		using MergeInplace = MergeImpl<T, S, merge_inplace>;

		template<typename T, typename S = std::less<T>>
		using Merge = MergeImpl<T, S, merge_simple>;
	}
}

#endif

