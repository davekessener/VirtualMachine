#include <sstream>
#include <cassert>
#include "Macro.h"
#include "Line.h"
#include "Token.h"
#include "Logger.h"
#include "ASMException.h"
#include "stringtools.h"

#define MXT_MACRO_SIG '%'

namespace vm { namespace assembler {

typedef std::vector<std::vector<Token>> param_vec;

namespace
{
	template<typename I>
	const std::string join(I i1, I i2)
	{
		std::string s;
		while(i1 != i2)
		{
			s += i1->str();
			++i1;
		}
		return s;
	}

	std::vector<std::vector<Token>> getParams(const Line&);
}

// # ===========================================================================

class Macro::Impl
{
	public:
		Impl(const std::string& n) : name_(n), insCount_(0) { }
	private:
		void expandToken(const Token&, Line&, const param_vec&) const;
		Line expandLine(const Line&, const param_vec&) const;
		bool isParam(const std::string&) const;
	private:
		friend class Macro;
		const std::string name_;
		std::vector<std::string> args_;
		std::vector<Line> cont_;
		mutable size_t insCount_;
};

// # ===========================================================================

Macro::Macro(const std::string& n, const std::vector<Token>& v) : impl_(new Impl(n))
{
	impl_->args_.reserve(v.size());
	for(auto i = v.cbegin() ; i != v.cend() ; ++i)
	{
		if(i->str()[0] != MXT_MACRO_SIG) MXT_LOGANDTHROW_T(*i, "ERR: No valid macro parametername.");
		else impl_->args_.push_back(i->str());
	}
}

Macro::~Macro(void) throw()
{
	delete impl_;
}

void Macro::addLine(const Line& line)
{
	if(line[0][0] == ':')
	{
		if(line.size() > 1) MXT_LOGANDTHROW_T(line[1], "ERR: Labels need be be on a seperate line!");
		else if(line[0].str().size() < 2) MXT_LOGANDTHROW_T(line[0], "ERR: Incomplete label!");
		else if(line[0][1] != ':') MXT_LOGANDTHROW_T(line[0], "ERR: Cannot define global label in macro. (Use '::' for local label instead)");
	}
	else
	{
		for(const Token& t : line)
		{
			if(t[0] == MXT_MACRO_SIG)
			{
				if(!impl_->isParam(t.str())) MXT_LOGANDTHROW_T(t, "ERR: Unknown parameter '%s'!", t.str().c_str());
			}
		}
	}

	impl_->cont_.push_back(line);
}

void Macro::expand(const Line& line, std::vector<Line>& outVec) const
{
	param_vec params(getParams(line));

	assert(params.size()==impl_->args_.size());

	for(const Line& ln : impl_->cont_)
	{
		outVec.push_back(impl_->expandLine(ln, params));
	}

	++(impl_->insCount_);
}

bool Macro::match(const Line& line) const
{
	if(line[0].str() != impl_->name_)
	{
//		LOG("[MAC] Name mismatch '%s' != '%s'", line[0].str().c_str(), impl_->name_.c_str());
		return false;
	}
	std::vector<Token> tmp(line.cbegin() + 1, line.cend());
	size_t c = 0;

	tmp.push_back(Token(",", line.filename(), line.line(), 
		tmp.empty() ? line[0].str().size() + 1 : tmp.back().word() + tmp.back().str().size() + 1));

	for(auto i = tmp.cbegin() ; i != tmp.cend() ; ++i)
	{
		if(i->str() == "," && i != tmp.cbegin()) ++c;
	}

	if(c != impl_->args_.size())
	{
//		LOG("[MAC] Argument count mismatch %lu != %lu", c, impl_->args_.size());
		return false;
	}

	LOG("[MAC] Matched macro '%s' in line %d.", impl_->name_.c_str(), line.line());

	return true;
}

const std::string Macro::signature(void) const
{
	std::ostringstream oss;
	oss << impl_->name_ << ":" << impl_->args_.size();
	return oss.str();
}

// # ===========================================================================

void Macro::Impl::expandToken(const Token& t, Line& l, const param_vec& params) const
{
	if(t[0] == MXT_MACRO_SIG)
	{
		for(size_t i = 0 ; i < args_.size() ; ++i)
		{
			if(t.str() == args_.at(i))
			{
				l += std::vector<Token>(params.at(i).begin(), params.at(i).end());
				return;
			}
		}

		MXT_LOGANDTHROW_T(t, "ERR: Couldn't match argument '%s'", t.str().c_str());
	}
	else if(t[0] == ':' && t[1] == ':')
	{
		std::ostringstream oss;
		oss << t.str() << "_" << name_ << ":" << std::to_string(args_.size()) 
			<< "_" << std::to_string(insCount_);
		l += Token(oss.str(), t.filename(), t.line(), t.word());
	}
	else
	{
		l += t;
	}
}

Line Macro::Impl::expandLine(const Line& ln, const param_vec& params) const
{
	Line l(ln.filename(), ln.line());

	for(const Token& t : ln)
	{
		expandToken(t, l, params);
	}

	LOG("[MAC] Subs. line '%s' -> '%s'", ln.str().c_str(), l.str().c_str());

	return l;
}

bool Macro::Impl::isParam(const std::string& s) const
{
	for(const std::string& p : args_)
	{
		if(s == p) return true;
	}

	return false;
}

// # ===========================================================================

namespace
{
	param_vec getParams(const Line& line)
	{
		param_vec params;
		std::vector<Token> raw(line.begin() + 1, line.end());

		raw.push_back(Token(",", line.filename(), line.line(), 
			raw.empty() ? line[0].str().size() + 1 : raw.back().word() + raw.back().str().size()));

		for(auto i = raw.begin(), j = i ; i != raw.end() ; ++i)
		{
			if(i->str() == ",")
			{
				if(i == j)
				{
					if(i == raw.begin()) break;
					else MXT_LOGANDTHROW_T(*i, "ERR: Malformed(empty) parameter");
				}

				params.push_back(std::vector<Token>(j, i));
				j = i + 1;
			}
		}

		return params;
	}
}

}}

