#ifndef VM_ASM_ADEF_H
#define VM_ASM_ADEF_H

#include <cstdint>
#include <ddef.h>

#define MXT_LOGANDTHROW(...) throw ::ASMException(::Logger::logFrom(__FILE__, __LINE__, __VA_ARGS__))
#define MXT_LOGANDTHROW_T(t,...) throw ::ASMException(::Logger::logFrom(__FILE__,__LINE__,"%s [['%s', '%s':%d,%d]]",::stringtools::string_format(__VA_ARGS__).c_str(),(t).str().c_str(),(t).filename().c_str(),(t).line(),(t).word()))

namespace vm
{
	namespace assembler
	{
		enum class Parameter : char
		{
			CONSTANT = 'C',
			REGISTER = 'R',
			MEMORY = 'M'
		};

		enum class Opcodes : std::uint8_t
		{
			EXPRESSION,
			MARKER,
			META
		};
	}
}

#endif

