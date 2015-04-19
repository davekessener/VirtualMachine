#include <algorithm>
#include <functional>	
#include <cassert>
#include "DEA.h"
#include "Reader.h"

namespace AF { namespace DEA {

namespace
{
	template<typename T, typename TT>
	inline bool contains(T&& c, const TT& v)
		{ return std::find(std::begin(c), std::end(c), v) != std::end(c); }

	template<typename T>
	inline bool isSubset(T&& c1, T&& c2)
	{
		for(const auto& e : c1)
		{
			if(!contains(c2, e)) return false;
		}

		return true;
	}
}

DEA DEA::Read(std::istream& in)
{
	return Reader::Generate(in);
}

// # ===========================================================================

StateTable DEA::generateTable(void) const
{
	StateTable tbl;
	std::set<StatePair> open;

	for(const auto& q : Q_)
	{
		for(const auto& p : Q_)
		{
			if(q != p)
			{
				tbl[StatePair(q, p)] = false;
			}
		}
	}

	for(const auto& q : Q_)
	{
		for(const auto& p : F_)
		{
			if(p != q && !contains(F_, q))
			{
				StatePair pq(q, p);
				tbl.at(pq) = true;
				open.emplace(std::move(pq));
			}
		}
	}

	while(!open.empty())
	{
		auto i = open.begin();

		for(const auto& a : S_)
		{
			std::set<State> l, r;

			for(const auto& d : d_)
			{
				if(d.second == i->first && d.first.second == a) l.insert(d.first.first);
				else if(d.second == i->second && d.first.second == a) r.insert(d.first.first);
			}

			for(const auto& p : l)
			{
				for(const auto& q : r)
				{
					StatePair pq(p, q);

					if(p != q && !tbl.at(pq))
					{
						tbl.at(pq) = true;
						open.emplace(std::move(pq));
					}
				}
			}
		}

		open.erase(i);
	}

	for(const auto& e : tbl)
	{
		if(!e.second)
		{
			open.insert(e.first);
		}
	}

	std::function<void(void)> run = [&](void)
	{
		for(const auto& e : open)
		{
			for(const auto& a : S_)
			{
				StatePair pq(d_.at(std::make_pair(e.first, a)), d_.at(std::make_pair(e.second, a)));

				if(pq.unique() && tbl.at(pq))
				{
					tbl.at(e) = true;
					open.erase(open.find(e));
					run();
					return;
				}
			}
		}
	};

	run();

	return tbl;
}

// # ===========================================================================

void Builder::addState(State q)
{
	auto f = Q_.emplace(std::move(q)).second; assert(f);
}

void Builder::addSymbol(Symbol a)
{
	auto f = S_.emplace(std::move(a)).second; assert(f);
}

void Builder::addRelation(State q, Symbol a, State p)
{
	if(!contains(Q_, q)) addState(q);
	if(!contains(Q_, p)) addState(p);
	if(!contains(S_, a)) addSymbol(a);
	auto f = d_.emplace(std::make_pair(std::move(q), std::move(a)), std::move(p)).second; assert(f);
}

void Builder::setInitialState(State q)
{
	if(!contains(Q_, q)) addState(q);

	q0_ = std::move(q);
}

void Builder::makeStateAccepting(State q)
{
	if(!contains(Q_, q)) addState(q);

	assert(!contains(F_, q));

	F_.emplace(std::move(q));
}

void Builder::fillDelta(const State &x)
{
	assert(contains(Q_, x));

	for(const auto& q : Q_)
	{
		for(const auto& a : S_)
		{
			auto k = std::make_pair(q, a);

			if(d_.find(k) == d_.end())
			{
				d_.emplace(std::move(k), State(x));
			}
		}
	}
}

DEA Builder::generate(void) const &
{
	check();

	return DEA(Builder(*this));
}

DEA Builder::generate(void) &&
{
	check();

	return DEA(std::move(*this));
}

void Builder::check(void) const
{
	assert(contains(Q_, q0_));
	assert(!Q_.empty());
	assert(!F_.empty());
	assert(!S_.empty());
	assert(isSubset(F_, Q_));
	assert(d_.size() == Q_.size() * S_.size());
}

} }

