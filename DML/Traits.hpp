#ifndef DML_TRAITS_H
#define DML_TRAITS_H

#include "DML.h"

namespace dml
{
	namespace traits
	{
		template<typename T>
		struct Strip
		{
			typedef T RET;
		};

		template<typename T>
		struct Strip<T *>
		{
			typedef T RET;
		};

		template<typename T>
		struct Strip<T&>
		{
			typedef T RET;
		};

		template<typename T>
		struct Strip<const T>
		{
			typedef T RET;
		};
	}
}

#endif

