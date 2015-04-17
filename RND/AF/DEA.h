#ifndef DAV_DEA_H
#define DAV_DEA_H

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <memory>

namespace AF
{
	template<typename T1, typename T2>
	class unordered_pair
	{
		public:
			unordered_pair(const T1& t1, const T2& t2)
				: first(t1 < t2 ? t1 : t2), second(t1 > t2 ? t1 : t2)
			{ }
			bool operator==(const unordered_pair<T1, T2>& p) const
				{ return (first == p.first && second == p.second) || (first == p.second && second == p.first); }
			bool operator<(const unordered_pair<T1, T2>& p) const
				{ return *this == p ? false : (first < p.first || (first == p.first && second < p.second)); }
			bool unique( ) const { return first != second; }
			T1 first;
			T2 second;
		private:
	};

	using Symbol = std::string;
	using State = std::string;
	using Alphabet = std::set<Symbol>;
	using States = std::set<State>;
	using Relations = std::map<std::pair<State, Symbol>, State>;
	using StatePair = unordered_pair<State, State>;
	using StateTable = std::map<StatePair, bool>;

	class DEA;

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
			DEA generate( ) const &;
			DEA generate( ) &&;
		private:
			void check( ) const;
		private:
			State q0_;
			Alphabet S_;
			States Q_, F_;
			Relations d_;
		private:
			friend class DEA;
	};

	class DEA
	{
		public:
			DEA(DEABuilder&& db)
				: q0_(db.q0_), S_(db.S_), Q_(db.Q_), F_(db.F_), d_(db.d_)
			{ }
			template<typename I>
				bool accepts(I, I) const;
			StateTable generateTable( ) const;
		private:
			State q0_;
			Alphabet S_;
			States Q_, F_;
			Relations d_;
	};

	class Reader
	{
		public:
			virtual std::string read( ) = 0;
			virtual bool empty( ) const = 0;
		private:
	};

	template<typename I>
	class AReader : public Reader
	{
		public:
			AReader(I i1, I i2) : i1_(i1), i2_(i2) { }
			virtual std::string read( ) override
			{
				if(empty()) throw std::string("ERR: Reader empty!");
				return *i1_++;
			}
			virtual bool empty( ) const override { return i1_ == i2_; }
		private:
			I i1_, i2_;
	};

	using Reader_ptr = std::shared_ptr<Reader>;

	template<typename I>
	Reader_ptr makeReader(I i1, I i2)
	{
		return std::dynamic_pointer_cast<Reader>(std::make_shared<AReader<I>>(i1, i2));
	}

	class DEAReader
	{
		public:
			static DEA Generate(Reader_ptr);
	};

	template<typename I>
	bool DEA::accepts(I i1, I i2) const
	{
		auto i = Q_.find(q0_);

		while(i1 != i2)
		{
			i = Q_.find(d_.at(std::make_pair(*i, static_cast<Symbol>(*i1)))); ++i1;
		}

		return F_.find(*i) != F_.end();
	}
}

#endif

