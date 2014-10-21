#ifndef DAV_MERGE_H
#define DAV_MERGE_H

#include "Sorting.hpp"

namespace dav
{
	namespace sorting
	{
		template<typename T, typename S = std::less<T>>
		class Merge
		{
			MXT_SORTING_CLASS(Merge);

			public:
				template<typename I>
				void operator()(I i1, I i2)
				{
				}
		};
	}
}

#endif

