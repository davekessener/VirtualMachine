#include "Tokenizer.h"
#include "Line.h"

namespace vm { namespace assembler {

Tokenizer::Tokenizer(const std::string& s) : str_(s), cp_(s.c_str()), wc_(0)
{
}

Tokenizer::~Tokenizer(void) throw()
{
}

Tokenizer& Tokenizer::operator>>(std::string& w)
{
	return *this;
}

Tokenizer& Tokenizer::operator>>(Line& line)
{
	return *this;
}

}}

