#ifndef VM_ASM_SYMTABLE_H
#define VM_ASM_SYMTABLE_H

#include <string>
#include <map>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		class SymTable
		{
			public:
			typedef std::map<std::string, WORD> lbl_map;
			typedef lbl_map::iterator iterator;
			typedef lbl_map::const_iterator const_iterator;

			public:
				SymTable( );
				~SymTable( ) throw();
				void insert(const std::string&, WORD);
//				WORD& operator[](const std::string&);
				bool knows(const std::string&) const;
				WORD get(const std::string&) const;
				void setOffset(int);
				void clear( );
				iterator begin( );
				iterator end( );
				const_iterator begin( ) const;
				const_iterator end( ) const;
				const_iterator cbegin( ) const;
				const_iterator cend( ) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

