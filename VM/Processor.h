#ifndef VM_PROCESSOR_H
#define VM_PROCESSOR_H

#include "ddef.h"
#include "CPU.h"

namespace vm
{
	namespace cpu
	{
		class Processor : public CPU
		{
			using CPU::commands_t;

			public:
			protected:
				void registerOpcodes(commands_t&);
			private:
		};
	}
}

#endif

