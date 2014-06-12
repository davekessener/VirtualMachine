#include "Instruction.h"
#include <sstream>
#include <iomanip>

namespace vm { namespace cpu {

Instruction::Instruction(const std::string& name, std::initializer_list<Arguments> args)
	: name_(name), args_(args.begin(), args.end())
{
}

Instruction::~Instruction(void)
{
}

const std::string Instruction::name(void) const
{
	return name_;
}

int Instruction::argCount(void) const
{
	return args_.size();
}

std::ostream& Instruction::print(std::ostream& os) const
{
	std::ostringstream oss;

	oss << std::left << std::setw(4) << name_ << std::setw(0) << " ";
	
	for(auto i = args_.cbegin() ; i != args_.cend() ;)
	{
		oss << static_cast<char>(*i);
		if(++i != args_.cend()) oss << ", ";
	}

	os << oss.str();
		
	return os;
}

}}

