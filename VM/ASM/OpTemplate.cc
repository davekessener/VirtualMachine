#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include "OpTemplate.h"
#include "Instruction.h"
#include "Evaluator.h"
#include "ASMException.h"
#include "Logger.h"
#include "Line.h"
#include "stringtools.h"

namespace vm { namespace assembler {

class OpTemplate::Impl
{
	typedef std::vector<Parameter> par_vec;

	public:
		Impl(WORD, const std::string&, par_vec);
		~Impl( ) throw();
		inline bool operator==(const Impl& i) const { return name_ == i.name_ && params_ == i.params_; }
		Opcode *match(const OpTemplate *, const Line& line) const;
		inline WORD id( ) const { return id_; }
		inline Parameter at(size_t idx) const { return params_.at(idx); }
		const std::string toString( ) const;
	private:
		friend class OpTemplate;
		WORD id_;
		std::string name_;
		par_vec params_;
};

// # ===========================================================================

OpTemplate::Impl::Impl(WORD id, const std::string& name, par_vec params)
	: id_(id), name_(name), params_(params)
{
}

OpTemplate::Impl::~Impl(void) throw()
{
}

Opcode *OpTemplate::Impl::match(const OpTemplate *that, const Line& line) const
{
	assert(line.size());

//	LOG("Trying to match '%s' to %s(0x%04x)", line.str().c_str(), name_.c_str(), id_);

	if(name_ != line[0].str())
	{
//		LOG("> Wrong name '%s'", line[0].c_str());
		return NULL;
	}

	std::vector<std::string> args;
	std::ostringstream oss;
	auto p = params_.begin();

	for(size_t i = 1 ; i < line.size() ; ++i)
	{
		if(i == line.size() - 1)
		{
			oss << line[i].str();
		}

		if(line[i].str() == "," || i == line.size() - 1)
		{
			std::string a(oss.str());

			if(a.empty() || (line[i].str() == "," && i == line.size() - 1))
				MXT_LOGANDTHROW_T(line[i], "ERR: Malformed/empty argument #%lu in '%s'",
					args.size() + 1, line.str().c_str());

			if(p == params_.end())
			{
//				LOG("> Too many parameters (%lu+ of %lu)", args.size() + 1, params_.size());
				return NULL;
			}

			Parameter pp = Evaluator::deduceParameter(a);
			if(pp != *p)
			{
//				LOG("> Wrong parameter type (%c instead %c)", pp, *p);
				return NULL;
			}

			++p;

			oss.str(std::string());
			oss.clear();

			args.push_back(a);

//			LOG("> Argument: '%s'(%c) matches", a.c_str(), pp);
		}
		else
		{
			oss << line[i].str();
		}
	}

	if(args.size() != params_.size())
	{
//		LOG("> Wrong argument count. (%lu instead of %lu)", args.size(), params_.size());
		return NULL;
	}

	LOG("> MATCH! With %s(0x%04x)", name_.c_str(), id_);

	return new Instruction(*that, args);
}

const std::string OpTemplate::Impl::toString(void) const
{
	std::ostringstream oss;

	oss << "0x" << std::setbase(16) << std::setfill('0') << std::setw(4)
		<< id_ << ": " << name_;
	
	bool f = false;
	for(Parameter p : params_)
	{
		if(f) oss << ",";
		oss << " " << static_cast<char>(p);
		f = true;
	}

	return oss.str();
}

// # ---------------------------------------------------------------------------

// # ===========================================================================

OpTemplate::OpTemplate(void) : impl_(NULL)
{
}

OpTemplate::OpTemplate(WORD id, const std::string& name, std::vector<Parameter> params)
	: impl_(new Impl(id, name, params))
{
}

OpTemplate::OpTemplate(const OpTemplate& ot) : impl_(ot.impl_ ? new Impl(*ot.impl_) : NULL)
{
}

OpTemplate::~OpTemplate(void) throw()
{
	delete impl_;
}

OpTemplate& OpTemplate::operator=(const OpTemplate& ot)
{
	OpTemplate t(ot);
	swap(t);
	return *this;
}

bool OpTemplate::operator==(const OpTemplate& op) const
{
	return (!impl_ && !op.impl_) || (impl_ && op.impl_ && *impl_ == *op.impl_);
}

void OpTemplate::swap(OpTemplate& ot) throw()
{
	Impl *i = impl_;
	impl_ = ot.impl_;
	ot.impl_ = i;
}

Opcode *OpTemplate::match(const Line& line) const
{
	return impl_ ? impl_->match(this, line) : NULL;
}

int OpTemplate::size(void) const
{
	int s = 1;

	for(const Parameter& p : impl_->params_)
	{
		switch(p)
		{
			case Parameter::CONSTANT:
			case Parameter::REGISTER:
				++s;
				break;
			case Parameter::MEMORY:
				s += 2;
				break;
			default:
				MXT_LOGANDTHROW("ERR: ??? Unknown parameter '%c'", p);
		}
	}

	return s;
}

WORD OpTemplate::id(void) const
{
	return impl_->id();
}

Parameter OpTemplate::operator[](size_t idx) const
{
	return impl_->at(idx);
}

const std::string& OpTemplate::name(void) const
{
	return impl_->name_;
}

const std::string OpTemplate::representation(void) const
{
	return impl_->toString();
}

}}

