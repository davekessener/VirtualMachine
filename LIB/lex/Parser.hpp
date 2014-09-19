#ifndef DAV_PARSER_H
#define DAV_PARSER_H

#include "lex.h"
#include "types.hpp"

namespace dav
{
	using lex::Empty;
	using lex::End;
	using lex::ID;
	

	typedef lex::Parser
	<
		MakeTypeList
		<
			MakeTypeList // 0
			<
				MakeTypeList<Translation<1>, End>
			>,
			MakeTypeList // 1
			<
				MakeTypeList<Translation<2>, Translation<4>>
			>,
			MakeTypeList // 2
			<
				MakeTypeList<Translation<3>, Translation<5>>
			>,
			MakeTypeList // 3
			<
				MakeTypeList<Literal<OPEN_PARA_S>, Translation<1>, Literal<CLOSE_PARA_S>>,
				MakeTypeList<ID<ID_DEC_REGEX>, PrintID>
			>,
			MakeTypeList // 4
			<
				MakeTypeList<Literal<ADD_S>, Translation<2>, Print<ADD_S>, Translation<4>>,
				MakeTypeList<Literal<SUB_S>, Translation<2>, Print<SUB_S>, Translation<4>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 5
			<
				MakeTypeList<Literal<MUL_S>, Translation<3>, Print<MUL_S>, Translation<5>>,
				MakeTypeList<Literal<DIV_S>, Translation<3>, Print<DIV_S>, Translation<5>>,
				MakeTypeList<Empty>
			>
		>,
		SVec
	>
	Parser;
}

#endif

