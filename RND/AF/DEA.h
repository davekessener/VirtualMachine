#ifndef DAV_DEA_H
#define DAV_DEA_H

#include <string>
#include <set>
#include <map>

namespace AF
{
	class DEABuilder;

	class DEA
	{
		using Symbol = std::string;
		using State = std::string;
		using Alphabet = std::set<Symbol>;
		using States = std::set<State>;
		using Relations = std::map<std::pair<State, Symbol>, State>;

		public:
			template<typename I>
				bool accepts(I, I) const;
		private:
			DEA(State q0, Alphabet S, States Q, States F, Relations d)
				: q0_(std::move(q0)), S_(std::move(S)), Q_(std::move(Q)), F_(std::move(F)), d_(std::move(d))
			{ }
		private:
			State q0_;
			Alphabet S_;
			States Q_, F_;
			Relations d_;
		private:
			friend class DEABuilder;
	};

	class DEABuilder
	{
		public:
			void addState(State);
			void addSymbol(Symbol);
			void addRelation(State, Symbol, State);
			template<typename I>
				void addRelations(const State& q, I i1, I i2, const State& p)
					{ while(i1 != i2) { addRelation(q, *i1, p); ++i1; } }
			void setInitialState(State);
			void makeStateAccepting(State);
			void fillDelta(const State&);
			DEA generate( ) &&;
		private:
			State q0_;
			Alphabet S_;
			States Q_, F_;
			Relations d_;
	};

	template<typename I>
	bool DEA::accepts(I i1, I i2) const
	{
		auto i = Q_.find(q0_);

		while(i1 != i2)
		{
			i = Q_.find(d_.at(std::make_pair(i->second, static_cast<Symbol>(*i1)))); ++i1;
		}

		return F_.find(i->second) != F_.end();
	}
}

#endif

