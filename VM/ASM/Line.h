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
				Line(const std::string&, const std::string&, int);
				~Line( ) throw();
				Line& operator=(Line&&);
				Line& operator=(const Line&);
				void swap(Line&) throw();
				Line& operator+=(const std::string&);
				Line& operator+=(const Token&);
				inline Line& operator+=(const char *s) { return *this += std::string(s); }
				template<typename C>
					Line& operator+=(const C& c) { for(const Token& t : c) *this += t; return *this; }
				Line& insert(const std::vector<std::string>&);
				inline explicit operator bool( ) const { return impl_ != NULL; }
				iterator begin( );
				iterator end( );
				const_iterator begin( ) const;
				const_iterator end( ) const;
				const_iterator cbegin( ) const;
				const_iterator cend( ) const;
				const std::string str( ) const;
				const std::string filename( ) const;
				int line( ) const;
				Token& operator[](size_t);
				const Token& operator[](size_t) const;
				size_t size( ) const;
				inline bool empty( ) const { return !size(); }
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

