#ifndef VM_ASM_TOKEN_H
#define VM_ASM_TOKEN_H

#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class Token
		{
			public:
				Token( );
				Token(Token&&);
				Token(const Token&);
				Token(const std::string&, const std::string&, int, int);
				~Token( ) throw();
				Token& operator=(Token&&);
				Token& operator=(const Token&);
				void swap(Token&) throw();
				bool operator==(const Token&) const;
				bool operator==(const std::string&) const;
				std::string& str( );
				const char *c_str( ) const;
				const std::string& str( ) const;
				const std::string filename( ) const;
				int line( ) const;
				int word( ) const;
				inline explicit operator bool( ) const { return impl_ != NULL; }
				char operator[](size_t) const;
			private:
				struct Impl;
				Impl *impl_;
		};

		inline bool operator==(const std::string& s, const Token& t)
		{
			return t == s;
		}
	}
}

#endif

