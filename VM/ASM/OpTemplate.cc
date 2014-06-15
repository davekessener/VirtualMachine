#include <iostream>
#include <sstream>
#include <iomanip>
#include "OpTemplate.h"
#include "Instruction.h"
#include "Evaluator.h"
#include "Logger.h"
#include "stringtools.h"

namespace vm { namespace assembler {

class OpTemplate::Impl
{
	typedef std::vector<Parameter> par_vec;

	public:
		Impl(WORD, const std::string&, par_vec);
		~Impl( ) throw();
		Opcode *match(const OpTemplate *, const std::string& line) const;
		inline int size( ) const { return params_.size(); }
		inline WORD id( ) const { return id_; }
		inline Parameter at(size_t idx) const { return params_.at(idx); }
		const std::string toString( ) const;
	private:
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

Opcode *OpTemplate::Impl::match(const OpTemplate *that, const std::string& line) const
{
	std::istringstream iss(line);
	std::string name;
	std::vector<std::string> args;

	LOG("Trying to match '%s' to %s(%d)", line.c_str(), name_.c_str(), params_.size());

	iss >> name;
	if(name != name_)
	{
		LOG("> Wrong name '%s'", name.c_str());
		return NULL;
	}

	auto i = params_.cbegin();

	while(iss)
	{
		std::string s;
		std::getline(iss, s, ',');
		trim(s);
		if(!s.empty())
		{
			if(i == params_.end())
			{
				LOG("> Too many parameters (%lu+)", args.size() + 1);
				return NULL;
			}

			if(Evaluator::deduceParameter(s) != *i)
			{
				LOG("> Wrong parameter type (%c instead %c)", Evaluator::deduceParameter(s), *i);
				return NULL;
			}

			++i;
			args.push_back(s);
		}
	}

	if(args.size() != params_.size())
	{
		LOG("> Not enough parameter (%lu)", args.size());
		return NULL;
	}

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

void OpTemplate::swap(OpTemplate& ot) throw()
{
	Impl *i = impl_;
	impl_ = ot.impl_;
	ot.impl_ = i;
}

Opcode *OpTemplate::match(const std::string& line) const
{
	return impl_ ? impl_->match(this, line) : NULL;
}

int OpTemplate::size(void) const
{
	return impl_->size();
}

WORD OpTemplate::id(void) const
{
	return impl_->id();
}

Parameter OpTemplate::operator[](size_t idx) const
{
	return impl_->at(idx);
}

const std::string OpTemplate::representation(void) const
{
	return impl_->toString();
}

}}

