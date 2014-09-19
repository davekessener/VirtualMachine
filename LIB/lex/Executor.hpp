#ifndef DAV_EVALUATOR_HPP
#define DAV_EVALUATOR_HPP

#include "lex.h"
#include "stack.hpp"
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
			MakeTypeList
			<
				MakeTypeList<Translation<1>, End>
			>,
			MakeTypeList
			<
				MakeTypeList<Translation<3>, Translation<2>>
			>,
			MakeTypeList
			<
				MakeTypeList<Translation<3>, Translation<2>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 3
			<
				MakeTypeList<ID<ID_DEC_REGEX>, Push>,
				MakeTypeList<Translation<4>>,
				MakeTypeList<Translation<5>>,
				MakeTypeList<Translation<6>>,
				MakeTypeList<Translation<7>>
			>,
			MakeTypeList
			<
				MakeTypeList<Literal<ADD_S>, Execute<Add>>
			>,
			MakeTypeList
			<
				MakeTypeList<Literal<SUB_S>, Execute<Sub>>
			>,
			MakeTypeList
			<
				MakeTypeList<Literal<MUL_S>, Execute<Mul>>
			>,
			MakeTypeList
			<
				MakeTypeList<Literal<DIV_S>, Execute<Div>>
			>
		>,
		StackHolder
	>
	Evaluator;
}

#endif

