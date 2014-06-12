#ifndef DML_INTEGRAL_H
#define DML_INTEGRAL_H

#include "DML.h"

namespace dml
{
	template<typename T, T V>
	struct Integral
	{
		static const T VAL = V;
		typedef Integral<T, V> RET;
		typedef Integral<T, V - 1> PREV;
		typedef Integral<T, V + 1> NEXT;
	};

	template<int I>
	struct Int
	{
		static const int VAL = I;
		typedef Int<I> RET;
		typedef Int<I - 1> PREV;
		typedef Int<I + 1> NEXT;
	};

	template<bool B>
	struct Bool
	{
		static const bool VAL = B;
		typedef Bool<B> RET;
	};

	typedef Bool<true> TRUE;
	typedef Bool<false> FALSE;
}

#endif

