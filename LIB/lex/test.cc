#include <iostream>
#include <vector>
#include <map>
#include "lex.h"

//using namespace dav::lex;

class ST
{
	public:
		explicit ST(std::ostream& os = std::cout) : os_(&os) { }
		std::string& operator[](const std::string& s) { return map_[s]; }
		std::string& at(const std::string& s) { return map_.at(s); }
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

typedef Parser
<
	ST,
	MakeTypeList
	<
		MakeTypeList
		<
			MakeTypeList<Number<1>, Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Number<2>, Number<4>, Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Number<3>, Number<5>, Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<OPEN_PARA_S>, Number<1>, Literal<CLOSE_PARA_S>, Empty>,
			MakeTypeList<ID<>, PrintID, Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<ADD_S>, Number<2>, Print<ADD_S>, Number<4>, Empty>,
			MakeTypeList<Literal<SUB_S>, Number<2>, Print<SUB_S>, Number<4>, Empty>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<MUL_S>, Number<3>, Print<MUL_S>, Number<5>, Empty>,
			MakeTypeList<Literal<DIV_S>, Number<3>, Print<DIV_S>, Number<5>, Empty>,
			MakeTypeList<Empty>
		>
	>
>
DParser;

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv + 1, argv + argc);

	DParser::parse(args.cbegin(), args.cend());

	std::cout << std::endl;

	return 0;
}

