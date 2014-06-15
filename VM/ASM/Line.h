#ifndef VM_ASM_LINE_H
#define VM_ASM_LINE_H

#include <string>
#include <vector>
#include "adef.h"
#include "Token.h"

namespace vm
{
	namespace assembler
	{
		class Line
		{
			typedef std::vector<Token> tok_vec;
			public:
			typedef tok_vec::iterator iterator;
			typedef tok_vec::const_iterator const_iterator;

			public:
				Line( );
				Line(Line&&);
				Line(const Line&);
				Line(const std::string&, int);
				~Line( ) throw();
				Line& operator=(Line&&);
				Line& operator=(const Line&);
				void swap(Line&) throw();
				Line& operator+=(const std::string&);
				Line& insert(const std::vector<std::string>&);
				inline explicit operator bool( ) const { return impl_ != NULL; }
				iterator begin( );
				iterator end( );
				const_iterator cbegin( ) const;
				const_iterator cend( ) const;
				const std::string str( ) const;
				int line( ) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

