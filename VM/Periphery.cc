#include "Periphery.h"

namespace vm { namespace periphery {

Periphery::Periphery(void)
{
}

Periphery::~Periphery(void)
{
}

void Periphery::link(cpu::CPU& cpu)
{
	cpu_ = &cpu;
}

}}

