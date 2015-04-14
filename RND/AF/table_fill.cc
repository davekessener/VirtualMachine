#include <iostream>

template<typename F>
class Tokenizer
{
	public:
		Tokenizer(F& f) : f_(&f) { }
		bool empty( ) const { return f_->feof(); }
		std::string read( ) { std::string s; (*f_) >> s; return s; }
	private:
		F *f_;
};

using State = std::string;
using Symbol = std::string;

class DEA
{
	public:
		void addTransition(State, Symbol, State);
		void finalize( );
		void minimize( );
		void merge(const DEA&);
		bool operator==(const DEA&) const;
	private:
		std::map<State, std::map<Symbol, State>> table_;
		std::set<State> states_;
		std::set<State> accepting_;
		std::set<Symbol> symbols_;
};

class StatePair
{
	public:
		StatePair(const State& s1, const State& s2) : s1_(s1), s2_(s2) {}
		bool operator==(const StatePair& p) const
			{ return (s1_ == p.s1_ && s2_ == p.s2_) || (s2_ == p.s1_ && s1_ == p.s2_); }
		bool operator<(const StatePair& p) const
			{ return s1_ < p.s1_; }
	private:
		std::string s1_, s2_;
};

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv + 1, argv + argc);
	Tokenizer t(std::cin);

	return 0;
}

void DEA::addTransition(State q, Symbol a, State p)
{
}

void DEA::finalize(void)
{
}

void DEA::minimize(void)
{
	std::map<StatePair, bool> tbl;
	std::vector<StatePair> open, closed;

	for(const auto& s1 : states_)
	{
		for(const auto& s2 : states_)
		{
			if(s1 != s2) tbl[StatePair(s1, s2)] = true;
		}
	}

	for(const auto& f : accepting_)
	{
		for(const auto& s : states_)
		{
			if(!accepting_.contains(s))
			{
				tbl[StatePair(f, s)] = false;
				open.push_back(StatePair(f, s));
			}
		}
	}

	for(const auto& s : open)
	{
	}
}

void DEA::merge(const DEA& dea)
{
}

bool DEA::operator==(const DEA& dea) const
{
}

