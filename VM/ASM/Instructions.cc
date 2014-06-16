#include <iostream>
#include <sstream>
#include <iomanip>
#include "Instructions.h"
#include "Reader.h"
#include "OpTemplate.h"
#include "Opcode.h"
#include "Logger.h"
#include "ASMException.h"
#include "Line.h"
#include "stringtools.h"

namespace vm { namespace assembler {

class Instructions::Impl
{
	public:
		Impl(std::istream&);
		~Impl( ) throw();
		Opcode *translate(const Line&) const;
	private:
		void addOpcode(const std::string&);
	private:
		std::vector<OpTemplate> ops_;
};

// # ===========================================================================

Instructions::Impl::Impl(std::istream& is)
{
	int lc = 0;
	std::string s;

	try
	{
		while(!is.eof())
		{
			std::getline(is, s);
			trim(s);
			if(!s.empty()) addOpcode(s);
			s = "";
			++lc;
		}
	}
	catch(const char *msg)
	{
		MXT_LOGANDTHROW("%s [[%d: '%s']]", msg, lc, s.c_str());
	}
}

Instructions::Impl::~Impl(void) throw()
{
}

Opcode *Instructions::Impl::translate(const Line& line) const
{
	Opcode *r = NULL;

	for(const OpTemplate &op : ops_)
	{
		if((r = op.match(line))) break;
	}

	if(!r) MXT_LOGANDTHROW("Couldn't translate '%s'", line.str().c_str());

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

	OpTemplate op(id, name, params);

	for(const OpTemplate &t : ops_)
	{
		if(t == op) throw "ERR: Duplicate opcode";
	}

	ops_.push_back(op);

//	LOG("Added new opcode: 0x%04x(%s)", id, name.c_str());
}

// # ===========================================================================

Instructions::Instructions(std::istream& is) : impl_(new Impl(is))
{
}

Instructions::~Instructions(void) throw()
{
	delete impl_;
}

Opcode *Instructions::translate(const Line& line) const
{
	return impl_->translate(line);
}

}}

