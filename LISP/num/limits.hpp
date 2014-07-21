#ifndef LIMITS_H
#define LIMITS_H

#include <limits>

namespace limits
{
	template<typename T, T V>
	struct value
	{
		typedef value<T, V> RET;
		static const T VAL = V;
	};

	template<typename T, T V, int L, int I, bool = false>
	struct max_power_impl
	{
		typedef typename max_power_impl<T, V / L, L, I + 1, V / L < L>::RET RET;
	};

	template<typename T, T V, int L, int I>
	struct max_power_impl<T, V, L, I, true>
	{
		typedef typename value<int, I>::RET RET;
	};

	template<typename T, int L>
	struct max_power
	{
		typedef typename max_power_impl<T, std::numeric_limits<T>::max(), L, 0>::RET RET;
		static const int VAL = RET::VAL;
	};
}

#endif

