#ifndef VM_PERIPHERY_H
#define VM_PERIPHERY_H

#include "ddef.h"

namespace vm
{
	namespace cpu
	{
		class CPU;
	}

	namespace periphery
	{
		class Periphery
		{
			public:
				Periphery( );
				virtual ~Periphery( );
				virtual void in(WORD) = 0;
				virtual WORD out( ) = 0;
				virtual void step( ) = 0;
				virtual void link(cpu::CPU&);
			protected:
				cpu::CPU *cpu_;
		};
	}
}

#endif

