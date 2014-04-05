#ifndef DML_TYPES_H
#define DML_TYPES_H

#include "DML.h"

namespace dml
{
	namespace types
	{
		template<typename T, typename U>
		struct Equal
		{
			struct RET { static int const VAL = 0; };
			enum { VAL = RET::VAL };
		};

		template<typename T>
		struct Equal<T, T>
		{
			struct RET { static int const VAL = 1; };
			enum { VAL = RET::VAL };
		};
	}
}

#endif

