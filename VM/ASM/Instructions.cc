#include <iostream>
#include <sstream>
#include <iomanip>
#include "Instructions.h"
#include "Reader.h"
#include "OpTemplate.h"
#include "Opcode.h"
#include "Logger.h"
#include "ASMException.h"

namespace vm { namespace assembler {

class Instructions::Impl
{
	public:
		Impl(std::istream&);
		~Impl( ) throw();
		Opcode *translate(const std::string&) const;
	private:
		void addOpcode(const std::string&);
	private:
		std::vector<OpTemplate> ops_;
};

// # ===========================================================================

Instructions::Impl::Impl(std::istream& is)
{
	Reader r(is);

	while(!r.empty())
	{
		addOpcode(r.getline());
	}
}

Instructions::Impl::~Impl(void) throw()
{
}

Opcode *Instructions::Impl::translate(const std::string& line) const
{
	Opcode *r = NULL;

	for(const OpTemplate &op : ops_)
	{
		Opcode *t = op.match(line);

		if(r && t)
			MXT_LOGANDTHROW("ERR: Duplicate signature: '%s' for '%s'!", 
				line.c_str(), op.representation().c_str());

		if(!r && t) r = t;
	}

	if(!r) MXT_LOGANDTHROW("Couldn't translate '%s'", line.c_str());

	return r;
}

// # ---------------------------------------------------------------------------

void Instructions::Impl::addOpcode(const std::string& line)
{
	auto error = [&line](int i)
		{
			MXT_LOGANDTHROW("ERR(%d): Malformed opcode '%s'", i, line.c_str());
		};

	std::istringstream iss(line);
	char c;
	WORD id;
	std::string name;
	std::vector<Parameter> params;

	iss >> c;
	if(c != '0') error(0);

	iss >> c;
	if(c != 'x') error(1);

	iss >> std::setbase(16) >> id;
	
	iss >> c;
	if(c != ':') error(2);
	
	iss >> name;

	while(iss)
	{
		c = '\0';
		iss >> c;
		switch(c)
		{
			case 'C': params.push_back(Parameter::CONSTANT); break;
			case 'R': params.push_back(Parameter::REGISTER); break;
			case 'M': params.push_back(Parameter::MEMORY); break;
			case ' ':
			case ',':
			case '\t':
			case '\0':
				break;
			default:
				std::cerr << "C==" << c << "(" << static_cast<int>(c) << ")" << std::endl;
				error(4);
				break;
		}

		if(!c) break;
	}

	ops_.push_back(OpTemplate(id, name, params));
}

// # ===========================================================================

Instructions::Instructions(std::istream& is) : impl_(new Impl(is))
{
}

Instructions::~Instructions(void) throw()
{
	delete impl_;
}

Opcode *Instructions::translate(const std::string& line) const
{
	return impl_->translate(line);
}

}}

