#ifndef VM_CPU_H
#define VM_CPU_H

#include <memory>
#include <vector>
#include <iostream>
#include "ddef.h"
#include <dav/Timer.h>

namespace vm
{
	namespace periphery
	{
		class Periphery;
	}

	namespace cpu
	{
		static const size_t RAM_SIZE = static_cast<OFFSET>(-1) + 1;
		static const size_t REG_COUNT = sizeof(WORD) << 3;

		class Instruction;

		enum class Interrupt
		{
			ENABLED,
			DISABLED,
			RUNNING
		};

		class CPU
		{
			public:
			typedef std::vector<std::shared_ptr<Instruction>> commands_t;
			typedef std::vector<std::shared_ptr<periphery::Periphery>> peripheries_t;

			public:
				CPU( );
				virtual ~CPU( );
				virtual void init( );
				void execute( );
				void step( );
				void reset( );
				void write(ADDRESS, size_t, const WORD *);
				void read(ADDRESS, size_t, WORD *);
				void push(WORD);
				WORD pop( );
				WORD& operator[](size_t);
				WORD next( );
				inline WORD *RAM( ) { return ram_; }
				inline const WORD *RAM( ) const { return ram_; }
				std::ostream& print(std::ostream&) const;
				const std::string printOpcodes( ) const;
				const std::string printRAM(ADDRESS, size_t) const;
				inline bool isRunning( ) const { return !isHalted_; }
				inline bool isSuspended( ) const { return isSuspended_; }
				void halt( ) { isHalted_ = true; }
				void suspend( ) { isSuspended_ = true; }
				void in(size_t, WORD);
				WORD out(size_t);
				void interrupt(int);
				void enableInterrupt(bool);
			protected:
				void attach(periphery::Periphery *);
				virtual void registerOpcodes(commands_t&) = 0;
				virtual void attachPeripherials( ) = 0;
			private:
				const Instruction& getIns( );
				const std::string printNextInstruction( ) const;
			private:
				WORD ram_[RAM_SIZE];
				WORD regs_[REG_COUNT];
				WORD &PC, &SP;
				commands_t commands_;
				peripheries_t peripheries_;
				bool isHalted_, isSuspended_;
				Interrupt interrupt_;
				dav::Timer t_;

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

