#include <cassert>
#include "DEA.h"

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

void DEABuilder::addState(State q)
{
	auto f = Q_.emplace(std::move(q)).second; assert(f);
}

void DEABuilder::addSymbol(Symbol a)
{
	auto f = S_.emplace(std::move(a)).second; assert(f);
}

void DEABuilder::addRelation(State q, Symbol a, State p)
{
	auto f = F_.emplace(std::make_pair(std::move(q), std::move(a)), std::move(p)).second; assert(f);
}

void DEABuilder::setInitialState(State q)
{
	assert(contains(Q_, q));

	q0_ = std::move(q);
}

void DEABuilder::makeStateAccepting(State q)
{
	assert(contains(Q_, q));
	assert(!contains(F_, q));

	F_.emplace(std::move(q));
}

void DEABuilder::fillDelta(const State &x)
{
	assert(contains(Q_, x));

	for(const auto& q : Q_)
	{
		for(const auto& a : S_)
		{
			auto k = std::make_pair(q, a);

			if(!contains(F_, k))
			{
				F_.emplace(std::move(k), State(x));
			}
		}
	}
}

DEA DEABuilder::generate(void) &&
{
	assert(contains(Q_, q0_));
	assert(!Q_.empty());
	assert(!F_.empty());
	assert(!S_.empty());
	assert(isSubset(F_, Q_));
	assert(F_.size() == Q_.size() * S_.size());

	return DEA(std::move(q0_), std::move(S_), std::move(Q_), std::move(F_), std::move(d_));
}

