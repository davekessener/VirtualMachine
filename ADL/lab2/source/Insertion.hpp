#ifndef DAV_INSERTION_H
#define DAV_INSERTION_H

#include "Sorting.hpp"

namespace dav
{
	namespace sorting
	{
		template<typename T, typename S = std::less<T>>
		class Insertion
		{
			MXT_SORTING_CLASS(Insertion);

			public:
				template<typename I>
				void operator()(I i1, I i2)
				{
					typedef std::reverse_iterator<I> riter;

					I begin(i1);

					while(++i1 != i2)
					{
						T t(*i1);
						I i(MXT_SORTING_UPPER_BOUND(begin, i1, t)), e(i1); ++e;

						if(i != i1)
						{
							MXT_SORTING_COPY(riter(i1), riter(i), riter(e));
							*i = t;
							MXT_SORTING_INCOP;
						}
					}
				}

		};
	}
}

#endif

