#ifndef DAV_TOKENIZER_H
#define DAV_TOKENIZER_H

#include "lex.h"
#include "types.hpp"

namespace dav
{
	using lex::Print;
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
				MakeTypeList<Translation<3>, Finalize, Translation<2>>
			>,
			MakeTypeList // 2
			<
				MakeTypeList<Translation<3>, Finalize, Translation<2>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 3
			<
				MakeTypeList<Translation< 4>>,
				MakeTypeList<Translation< 6>>,
				MakeTypeList<Translation<12>>
			>,
			MakeTypeList // 4
			<
				MakeTypeList<ID<ID_IDT_BEG_REGEX>, PrintID, Translation<5>>
			>,
			MakeTypeList // 5
			<
				MakeTypeList<ID<ID_IDT_END_REGEX>, PrintID, Translation<5>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 6
			<
				MakeTypeList<Translation<8>, Translation<10>, Translation<11>>
			>,
			MakeTypeList // 7
			<
				MakeTypeList<Literal<ADD_S>, Translation<8>>,
				MakeTypeList<Literal<SUB_S>, Print<SUB_S>, Translation<8>>,
				MakeTypeList<Translation<8>>
			>,
			MakeTypeList // 8
			<
				MakeTypeList<ID<ID_DIGIT_REGEX>, PrintID, Translation<9>>
			>,
			MakeTypeList // 9
			<
				MakeTypeList<ID<ID_DIGIT_REGEX>, PrintID, Translation<9>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 10
			<
				MakeTypeList<Literal<DOT_S>, Print<DOT_S>, Translation<8>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 11
			<
				MakeTypeList<ID<ID_EXP_REGEX>, PrintID, Translation<7>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 12
			<
				MakeTypeList<ID<ID_WS_REGEX>>,
				MakeTypeList<ID<ID_OP_REGEX>, PrintID>
			>
		>,
		Printer
	>
	Tokenizer;
}

#endif

