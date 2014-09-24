#ifndef VM_ASM_OPTEMPLATE_H
#define VM_ASM_OPTEMPLATE_H

#include <string>
#include <vector>
#include "adef.h"
#include "Opcode.h"

namespace vm
{
	namespace assembler
	{
		struct Line;

		class OpTemplate
		{
			public:
				OpTemplate( );
				OpTemplate(WORD, const std::string&, std::vector<Parameter>);
				OpTemplate(const OpTemplate&);
				~OpTemplate( ) throw();
				OpTemplate& operator=(const OpTemplate&);
				bool operator==(const OpTemplate&) const;
				void swap(OpTemplate&) throw();
				Opcode *match(const Line&) const;
				int size( ) const;
				WORD id( ) const;
				Parameter operator[](size_t) const;
				const std::string& name( ) const;
				const std::string representation( ) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

