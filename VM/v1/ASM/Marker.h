#ifndef VM_ASM_MARKER_H
#define VM_ASM_MARKER_H

#include <string>
#include "adef.h"
#include "Opcode.h"

namespace vm
{
	namespace assembler
	{
		class Marker : public Opcode
		{
			public:
				explicit Marker(const std::string&);
				virtual ~Marker( ) throw();
				virtual void out(std::ostream&);
				virtual int size( ) const;
				virtual const op_proxy evaluate(SymTable&, int) const;
				const std::string name( ) const { return name_; }
			private:
				std::string name_;
		};
	}
}

#endif

