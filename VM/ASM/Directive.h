#ifndef VM_SM_DIRECTIVE_H
#define VM_SM_DIRECTIVE_H

#include "adef.h"
#include "Opcode.h"

namespace vm
{
	namespace assembler
	{
		class Directive : public Opcode
		{
			public:
				Directive( );
				virtual ~Directive( ) throw();
				virtual void out(std::ostream&);
				virtual int size( ) const;
				virtual const op_proxy evaluate(SymTable&, int) const;
			private:
		};
	}
}

#endif

