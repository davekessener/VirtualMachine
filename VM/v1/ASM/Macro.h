#ifndef VM_ASM_MACRO_H
#define VM_ASM_MACRO_H

#include <vector>
#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct Line;
		struct Token;

		class Macro
		{
			public:
				Macro(const std::string&, const std::vector<Token>&);
				~Macro( ) throw();
				void addLine(const Line&);
				void expand(const Line&, std::vector<Line>&) const;
				bool match(const Line&) const;
				const std::string signature( ) const;
				inline bool operator==(const Macro& m) const { return signature() == m.signature(); }
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

