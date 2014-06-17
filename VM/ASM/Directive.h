#ifndef VM_SM_DIRECTIVE_H
#define VM_SM_DIRECTIVE_H

#include <string>
#include "adef.h"
#include "Opcode.h"

namespace vm
{
	namespace assembler
	{
		class DirectiveDB : public Opcode
		{
			public:
				DirectiveDB(const std::string&);
				virtual ~DirectiveDB( ) throw();
				virtual void out(std::ostream&);
				virtual int size( ) const;
				virtual const op_proxy evaluate(SymTable&, int) const;
			private:
				const std::string expr_;
		};
	}
}

#endif

