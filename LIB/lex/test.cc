#include <iostream>
#include <vector>
#include <map>
#include "lex.h"

using namespace dav::lex;

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

//typedef Parser
//<
//	ST,
//	MakeTypeList
//	<
//		MakeTypeList
//		<
//			MakeTypeList<Number<1>, Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Number<2>, Number<4>, Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Number<3>, Number<5>, Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Literal<"(">, Number<1>, Literal<")">, Empty>,
//			MakeTypeList<ID, PrintID, Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Literal<"+">, Number<2>, Print<"+">, Number<4>, Empty>,
//			MakeTypeList<Literal<"-">, Number<2>, Print<"-">, Number<4>, Empty>,
//			MakeTypeList<Empty>
//		>,
//		MakeTypeList
//		<
//			MakeTypeList<Literal<"*">, Number<3>, Print<"*">, Number<5>, Empty>,
//			MakeTypeList<Literal<"/">, Number<3>, Print<"/">, Number<5>, Empty>,
//			MakeTypeList<Empty>
//		>
//	>
//>
//DParser;

int main(int argc, char *argv[])
{
	typedef ID<>::Make<ST>::type DID;
	typedef PrintID::Make<ST>::type PR;
	std::vector<std::string> args(argv, argv + argc);
	ST st;
	auto i1 = args.begin(), i2 = args.end();

	while(i1 != i2)
	{
		DID::matches(st, i1, i2);
		PR::matches(st, i1, i2);
		std::cout << std::endl;
	}
	return 0;
}

