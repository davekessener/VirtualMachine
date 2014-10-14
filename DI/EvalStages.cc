#include "EvalStages.h"
#include "BitBuffer.hpp"
#include <map>
#include <stack>
#include <iomanip>

namespace dav { namespace dil {

void Stage0_Name::parse(const std::string& token)
{
	next(new Stage1_Vars(token));
}

void Stage1_Vars::parse(const std::string& token)
{
	if(token == "#")
	{
		next(new Stage2_Equation(name_, std::move(vars_)));
	}
	else
	{
		vars_.push_back(token);
	}
}

void Stage2_Equation::parse(const std::string& token)
{
	if(token == "$")
	{
		next(new Stage3_Evaluation(name_,
						vars_.cbegin(), vars_.cend(), 
						eqs_.cbegin(), eqs_.cend()));
	}
	else
	{
		eqs_.push_back(token);
	}
}

namespace
{
	class cw_t
	{
			typedef std::function<void(const std::string&)> push_fn;
		public:
			struct eol_t { };
			cw_t(push_fn p) : push_(p) { }
			~cw_t( ) { flush(); }
			template<typename T> cw_t& operator<<(T&& t) { ss_ << t; return *this; }
			cw_t& operator<<(eol_t *) { flush(); return *this; }
			void flush( ) { if(!ss_.str().empty()) push_(ss_.str()); ss_.clear(); ss_.str(""); }
		private:
			push_fn push_;
			std::stringstream ss_;
	};

	cw_t::eol_t *eol = nullptr;
}

template<typename I1, typename I2>
	Stage3_Evaluation::Stage3_Evaluation(const std::string& name, I1 i11, I1 i12, I2 i21, I2 i22)
{
	cw_t cw([this](const std::string& s) { out_.push_back(s); });

	cw << "# =======================================================================" << eol;
	cw << "# --- " << std::left;

	std::map<std::string, int> lookup;
	size_t ms = 0;

	for(int i = 0 ; i11 != i12 ; ++i11, ++i) { lookup[*i11] = i; if(i11->size() > ms) ms = i11->size(); }

	++ms;

	for(const auto& p : lookup)
	{
		cw << std::setw(ms) << p.first;
	}

	cw << std::right << "| " << name << eol;
	cw << "# =======================================================================" << eol;

	bitbuffer::BitBuffer bb(lookup.size());
	std::vector<std::string> equ(i21, i22);
	std::stack<bool> bs;
	std::map<std::string, std::function<void(void)>> ops;

	ops["!"] = [&bs]() { bool f(bs.top()); bs.pop(); bs.push(!f); };
	ops["&"] = [&bs]() { bool v1(bs.top()); bs.pop(); bool v2(bs.top()); bs.pop(); bs.push(v1 && v2); };
	ops["|"] = [&bs]() { bool v1(bs.top()); bs.pop(); bool v2(bs.top()); bs.pop(); bs.push(v1 || v2); };

	do
	{
		for(const std::string& op : equ)
		{
			if(ops.find(op) != ops.end())
			{
				ops[op]();
			}
			else if(lookup.find(op) != lookup.end())
			{
				bs.push(bb.get(lookup[op]));
			}
			else
			{
				throw std::string("Unknown symbol '" + op + "'!");
			}
		}

		if(bs.size() != 1) throw std::string("Invalid size!");

		cw << "# --- " << std::left;

		for(int i = 0 ; i < (long)lookup.size() ; ++i)
		{
			cw << std::setw(ms) << bb.get(i);
		}

		cw << std::right << "| " << bs.top() << eol;

		bs.pop();

		bb.increment();
	}
	while(!bb.empty());

	cw << "# =======================================================================" << eol;
}

}}

