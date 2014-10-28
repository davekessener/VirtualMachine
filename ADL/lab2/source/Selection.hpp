#ifndef DAV_SELECTION_H
#define DAV_SELECTION_H

#include "Sorting.hpp"

namespace dav
{
	namespace sorting
	{
		template<typename T, typename S = std::less<T>>
		class Selection
		{
			MXT_SORTING_CLASS(Selection);

			public:
				template<typename I>
				void operator()(I i1, I i2)
				{
					while(i1 != i2)
					{
						I i(MXT_SORTING_MIN_ELEMENT(i1, i2));
						if(i != i1) MXT_SORTING_SWAP(*i, *i1);
						++i1;
					}
				}
		};
	}
}

#endif

