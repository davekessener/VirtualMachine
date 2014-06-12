#include "CPU.h"
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>

namespace vm { namespace cpu {

CPU::CPU(void) : PC(regs_[0]), SP(regs_[REG_COUNT - 1])
{
}

CPU::~CPU(void)
{
}

void CPU::run(void)
{
	reset();

	for(;;)
	{
		if(ram_[PC] >= commands_.size())
		{
			break;
		}

		if(!(getIns()(*this)))
		{
			break;
		}
	}
}

void CPU::reset(void)
{
	for(size_t i = 0 ; i < REG_COUNT ; ++i)
	{
		regs_[i] = 0;
	}

	commands_.clear();

	registerOpcodes(commands_);
}

void CPU::write(ADDRESS a, size_t s, const WORD *src)
{
	assert(a + s < RAM_SIZE);

	WORD *ram = ram_ + a;
	while(s--) *ram++ = *src++;
}

void CPU::read(ADDRESS a, size_t s, WORD *dst)
{
	assert(a + s < RAM_SIZE);

	const WORD *ram = ram_ + a;
	while(s--) *dst++ = *ram++;
}

void CPU::push(WORD v)
{
	ram_[--SP] = v;
}

WORD CPU::pop(void)
{
	return ram_[SP++];
}

WORD& CPU::operator[](size_t idx)
{
	assert(idx<REG_COUNT);
	return regs_[idx];
}

WORD CPU::next(void)
{
	return ram_[PC++];
}

std::ostream& CPU::print(std::ostream& os) const
{
	char buf[1024];

	std::ostringstream oss;

	sprintf(buf, "#===============================================================================\n"
				 "#--- Registers of %lu bit CPU ---------------------------------------------------\n"
				 "#===============================================================================\n",
				 REG_COUNT);

	oss << std::string(buf);

#define MXT_STEP 4
	for(size_t i = 0, l = (REG_COUNT + MXT_STEP - 1) / MXT_STEP ; i < l ; ++i)
	{
		std::ostringstream ss;

		ss << "#--- ";

		for(size_t j = 0 ; j < MXT_STEP && i + j * l < REG_COUNT ; ++j)
		{
			sprintf(buf, "[%02lx] 0x%04x", i + j * l, regs_[i + j * l]);
			ss << buf << '\t';
		}

		oss << ss.str() << std::endl;
	}
#undef MXT_STEP

	os << oss.str();

	return os;
}

const std::string CPU::printOpcodes(void) const
{
	std::ostringstream oss;
	char buf[512];

	for(size_t i = 0, l = commands_.size() ; i < l ;)
	{
		sprintf(buf, "0x%02lx: ", i);
		oss << std::string(buf) << *commands_.at(i);
		if(++i < l) oss << std::endl;
	}

	return oss.str();
}

const Instruction& CPU::getIns(void)
{
	return *commands_.at(next());
}

}}

