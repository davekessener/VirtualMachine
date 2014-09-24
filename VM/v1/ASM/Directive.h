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
				virtual ~DirectiveDB( ) throw() { }
				virtual void out(std::ostream&);
				virtual int size( ) const { return 1; }
				virtual const op_proxy evaluate(SymTable&, int) const;
			private:
				const std::string expr_;
		};

		class DirectiveORG : public Opcode
		{
			public:
				DirectiveORG(WORD);
				virtual ~DirectiveORG( ) throw() { }
				virtual void out(std::ostream&);
				virtual int size( ) const { return 0; }
				virtual const op_proxy evaluate(SymTable&, int) const;
				inline WORD getOffset( ) const { return off_; }
			private:
				WORD off_;
		};
	}
}

#endif

