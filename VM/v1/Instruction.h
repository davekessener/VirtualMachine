#ifndef VM_INSTRUCTION_H
#define VM_INSTRUCTION_H

#include <string>
#include <vector>
#include <initializer_list>
#include <iostream>
#include "ddef.h"

namespace vm
{
	namespace cpu
	{
		class CPU;

		enum class Arguments : char
		{
			CONSTANT = 'C',
			REGISTER = 'R',
			MEMORY = 'M'
		};

		class Instruction
		{
			public:
				Instruction(const std::string&, std::initializer_list<Arguments>);
				virtual ~Instruction( );
				virtual void operator()(CPU&) const = 0;
				const std::string name( ) const;
				int argCount( ) const;
				const Arguments operator[](size_t i) const { return args_[i]; }
				std::ostream& print(std::ostream&) const;
			private:
				std::string name_;
				std::vector<Arguments> args_;
		};

		inline std::ostream& operator<<(std::ostream& os, const Instruction& ins)
		{
			return ins.print(os);
		}
	}
}

#endif

