#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include "lex.h"
#include <dav/Logger.h>

//using namespace dav::lex;

class ST
{
	public:
		explicit ST(std::ostream& os = std::cout) : os_(&os) { }
		std::string& operator[](const std::string& s) { LOG("Accessed %s", s.data()); return map_[s]; }
		std::string& at(const std::string& s) { LOG("Bound access to %s", s.data()); return map_.at(s); }
		ST& operator<<(const std::string& s) { *os_ << s; return *this; }
	private:
		std::map<std::string, std::string> map_;
		std::ostream *os_;
};

typedef String<'('> OPEN_PARA_S;
typedef String<')'> CLOSE_PARA_S;
typedef String<'+'> ADD_S;
typedef String<'-'> SUB_S;
typedef String<'*'> MUL_S;
typedef String<'/'> DIV_S;
typedef String<'[', '0', '-', '9', ']', '+'> ID_NUM_REGEX;
typedef String<'[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> ID_DEC_REGEX;

typedef String<'[', '0', '-', '9', ']'> ID_DIGIT_REGEX;
typedef String<'.'> DOT_S;
typedef String<' '> WS_S;
typedef String<'[', 'e', 'E', ']'> ID_EXP_REGEX;

template<int V>
using Translation = Number<V>;

typedef Parser
<
	MakeTypeList
	<
		MakeTypeList // 0
		<
			MakeTypeList<Translation<1>>
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
			MakeTypeList<Translation<6>>
//			MakeTypeList<ID<ID_NUM_REGEX>, PrintID>
		>,
		MakeTypeList // 4
		<
			MakeTypeList<Literal<ADD_S>, Translation<2>, Print<ADD_S>, Print<WS_S>, Translation<4>>,
			MakeTypeList<Literal<SUB_S>, Translation<2>, Print<SUB_S>, Print<WS_S>, Translation<4>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList // 5
		<
			MakeTypeList<Literal<MUL_S>, Translation<3>, Print<MUL_S>, Print<WS_S>, Translation<5>>,
			MakeTypeList<Literal<DIV_S>, Translation<3>, Print<DIV_S>, Print<WS_S>, Translation<5>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList // 6
		<
			MakeTypeList<Translation<7>, Translation<10>, Translation<11>>
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
			MakeTypeList<Print<WS_S>, Empty>
		>,
		MakeTypeList // 10
		<
			MakeTypeList<Literal<DOT_S>, Print<DOT_S>, Translation<8>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList // 11
		<
			MakeTypeList<ID<ID_EXP_REGEX>, PrintID, Translation<8>>,
			MakeTypeList<Empty>
		>
	>
>
DParser;

// # ===========================================================================

//typedef String<'[', '0', '-', '9', ']'> ID_DIGIT_REGEX;
//typedef String<' '> WS_S;
//typedef String<'.'> DOT_S;
//typedef String<'[', 'e', 'E', ']'> ID_EXP_REGEX;
//typedef String<'[', '+', '\\', '-', '\\', '*', '/', ']'> ID_OP_REGEX;
//
//typedef Parser
//<
//	MakeTypeList
//	<
//		MakeTypeList
//		<
//			MakeTypeList<Translation<1>>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Translation<3>, Print<WS_S>, Translation<2>>,
//			MakeTypeList<Translation<4>, Translation<2>>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Translation<1>>,
//			MakeTypeList<Empty>
//		>,
//		MakeTypeList // 3
//		<
//			MakeTypeList<Translation<5>, Translation<6>>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<ID<ID_OP_REGEX>, PrintID, Print<WS_S>, Translation<9>>
//		>,
//		MakeTypeList // 5
//		<
//			MakeTypeList<ID<ID_DIGIT_REGEX>, PrintID, Translation<8>>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Literal<DOT_S>, Print<DOT_S>, Translation<5>, Translation<7>>,
//			MakeTypeList<Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<ID<ID_EXP_REGEX>, PrintID, Translation<5>>,
//			MakeTypeList<Empty>
//		>,
//		MakeTypeList // 8
//		<
//			MakeTypeList<ID<ID_DIGIT_REGEX>, PrintID, Translation<8>>,
//			MakeTypeList<Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Literal<OPEN_PARA_S>, Print<OPEN_PARA_S>, Print<WS_S>, Translation<1>, Literal<CLOSE_PARA_S>, Print<CLOSE_PARA_S>, Print<WS_S>>,
//			MakeTypeList<Translation<3>>
//		>
//	>
//>
//Tokenizer;

// # ===========================================================================

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	if(args.size() != 2) throw std::string("ERR: Need to supply exactly one term!");

	std::vector<std::string> term(args.at(1).size());
	std::copy(args.at(1).cbegin(), args.at(1).cend(), term.begin());

	DParser::parse(term.cbegin(), term.cend(), std::cout);

	std::cout << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr<< e << std::endl;
}

