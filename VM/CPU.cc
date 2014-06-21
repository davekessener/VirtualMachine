#include <iomanip>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "CPU.h"
#include "Instruction.h"
#include "Periphery.h"
#include "Logger.h"

#define MXT_FRAMERATE 60

namespace vm { namespace cpu {

using periphery::Periphery;

CPU::CPU(void) : PC(regs_[REG_COUNT - 1]), SP(regs_[REG_COUNT - 2]), 
	isHalted_(false), isSuspended_(false), interrupt_(Interrupt::DISABLED)
{
}

CPU::~CPU(void)
{
}

void CPU::init(void)
{
	registerOpcodes(commands_);
	attachPeripherials();
}

void CPU::execute(void)
{
	if(isHalted_) return;

	for(auto i : peripheries_)
	{
		i->step();
	}

	if(isSuspended_)
	{
		t_.keepRate(MXT_FRAMERATE);
		return;
	}

	Logger::log(printNextInstruction());

	if(ram_[PC] >= commands_.size())
	{
		isHalted_ = true;
	}
	else
	{
		getIns()(*this);
	}

	if(interrupt_ == Interrupt::RUNNING) interrupt_ = Interrupt::ENABLED;
}

void CPU::reset(void)
{
	for(size_t i = 0 ; i < REG_COUNT ; ++i)
	{
		regs_[i] = 0;
	}

	isHalted_ = false;
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
	for(size_t i = 0, l = (REG_COUNT + MXT_STEP - 1) / MXT_STEP ; i < l ;)
	{
		std::ostringstream ss;

		ss << "#---\t";

		for(size_t j = 0 ; j < MXT_STEP && i + j * l < REG_COUNT ; ++j)
		{
			sprintf(buf, "[%02lx] 0x%04x", i + j * l, regs_[i + j * l]);
			ss << buf << '\t';
		}

		oss << ss.str();

		if(++i < l) oss << std::endl;
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

const std::string CPU::printRAM(ADDRESS a, size_t s) const
{
	assert(a+s<RAM_SIZE);

	char buf[128];
	std::ostringstream oss;

	sprintf(buf, "@[0x%04x - 0x%04lx](0x%04lx) ", a, a + s, s);
	oss << std::setfill('=') << std::setw(80) << std::left << "#" << std::setw(0) << std::endl
		<< std::setfill('-') << std::setw(80) << (std::string("#--- RAM ") += buf) << std::setw(0) << std::endl
		<< std::setfill('=') << std::setw(80) << "#" << std::setw(0) << std::endl;
	
	oss << "       | " << std::setbase(16) << std::setfill('0') << std::right;
	for(int i = 0 ; i < 0x10 ; ++i) oss << std::setw(4) << i << std::setw(0) << " ";
	oss << std::endl << std::setfill('-') << std::setw(88) << std::left << "-------+" << std::setw(0);

	for(size_t i = a / 0x10 ; i < (a + s + 0x0f) / 0x10 ; ++i)
	{
		std::ostringstream line;
		sprintf(buf, "0x%04lx | ", i * 0x10);
		line << std::endl << std::string(buf);

		for(size_t j = 0 ; j < 0x10 ; ++j)
		{
			if(j + i * 0x10 < a || j + i * 0x10 >= a + s)
			{
				line << "    ";
			}
			else
			{
				sprintf(buf, "%04x", ram_[j + i * 0x10]);
				line << std::string(buf);
			}

			line << " ";
		}

		oss << line.str();
	}

	return oss.str();
}

void CPU::in(size_t idx, WORD v)
{
	if(idx < peripheries_.size())
	{
		peripheries_.at(idx)->in(v);
	}
}

WORD CPU::out(size_t idx)
{
	return (idx < peripheries_.size()) ? peripheries_.at(idx)->out() : 0;
}

void CPU::interrupt(int id)
{
	if(interrupt_ == Interrupt::ENABLED)
	{
		push(PC);
		PC = ram_[INT_TABLE + id];
		interrupt_ = Interrupt::RUNNING;
		isSuspended_ = false;
	}
}

void CPU::enableInterrupt(bool f)
{
	interrupt_ = (f && interrupt_ != Interrupt::RUNNING) ? Interrupt::ENABLED : Interrupt::DISABLED;
}

// # ---------------------------------------------------------------------------

void CPU::attach(Periphery *p)
{
	peripheries_.push_back(std::shared_ptr<Periphery>(p));
	p->link(*this);
}

// # ===========================================================================

const Instruction& CPU::getIns(void)
{
	return *commands_.at(next());
}

const std::string CPU::printNextInstruction(void) const
{
	char buf[128];
	std::ostringstream oss;

	if(ram_[PC] >= commands_.size())
	{
		oss << "### ERR: Unknown command ";
		sprintf(buf, "[@0x%04x]: 0x%04x", PC, ram_[PC]);
		oss << std::string(buf);
	}
	else
	{
		auto i = commands_.at(ram_[PC]);
		oss << "### ";
		sprintf(buf, "@0x%04x: ", PC);
		oss << std::string(buf) << std::setw(4) << i->name() << std::setw(0);
		sprintf(buf, "(0x%04x)", ram_[PC]);
		oss << std::string(buf);
		
		const WORD *ram = ram_ + PC + 1;
		for(int j = 0 ; j < i->argCount() ; ++j)
		{
			if(j) oss << ",";
			sprintf(buf, " 0x%04x", *ram++);
			oss << std::string(buf);

			if((*i)[j] == Arguments::MEMORY)
			{
				sprintf(buf, "+0x%04x", *ram++);
				oss << std::string(buf);
			}
		}
	}

	return oss.str();
}

}}

