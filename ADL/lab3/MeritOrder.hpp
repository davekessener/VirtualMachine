#ifndef DAV_MERITORDER_H
#define DAV_MERITORDER_H

#include "PQueue.hpp"

namespace dav
{
	typedef std::pair<double, double> mo;

	namespace merit
	{
		struct cmp
		{
			bool operator()(const mo& v1, const mo& v2) const { return v1.first < v2.first; }
		};
	}

	template<typename I, typename O>
	void merit_order(I i1, I i2, O o)
	{
		PQueue<mo, merit::cmp> pq(i1, i2);
		mo v(0, 0);

		while(!pq.empty())
		{
			v.first += pq.top().first;
			v.second += pq.top().second;

			pq.remove();

			*o = v;
			++o;
		}
	}
}

#endif

