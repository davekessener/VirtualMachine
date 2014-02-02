#ifndef PAIR_H
#define PAIR_H

template<typename T1, typename T2>
class Pair
{
	public:
		Pair(const T1& _first, const T2& _second) :
			first(_first), 
			second(_second)
		{
		}

		Pair(T1&& _first, T2&& _second) :
			first(_first), 
			second(_second)
		{
		}

		Pair(const Pair<T1, T2>& pair) :
			first(pair.first),
			second(pair.second)
		{
		}

		const T1 first;
		const T2 second;
	private:
		Pair<T1, T2>& operator=(const Pair<T1, T2>&);
};

#endif

