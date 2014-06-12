#ifndef VM_CPU_H
#define VM_CPU_H

#include "ddef.h"
#include "Instruction.h"
#include <memory>

namespace vm
{
	namespace cpu
	{
		static const size_t RAM_SIZE = static_cast<OFFSET>(-1) + 1;
		static const size_t REG_COUNT = sizeof(WORD) << 3;

		class CPU
		{
			public:
			typedef std::vector<std::shared_ptr<Instruction>> commands_t;

			public:
				CPU( );
				virtual ~CPU( );
				void run( );
				virtual void reset( );
				void write(ADDRESS, size_t, const WORD *);
				void read(ADDRESS, size_t, WORD *);
				void push(WORD);
				WORD pop( );
				WORD& operator[](size_t);
				WORD next( );
				std::ostream& print(std::ostream&) const;
				const std::string printOpcodes( ) const;
			protected:
				virtual void registerOpcodes(commands_t&) = 0;
			private:
				const Instruction& getIns( );
			private:
				WORD ram_[RAM_SIZE];
				WORD regs_[REG_COUNT];
				WORD &PC, &SP;
				commands_t commands_;

			private:
				CPU(const CPU&);
				CPU& operator=(const CPU&);
		};

		inline std::ostream& operator<<(std::ostream& os, const CPU& cpu)
		{
			return cpu.print(os);
		}
	}
}

#endif

