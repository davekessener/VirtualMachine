//#include <iostream>
//#include "Tokenizer.h"
//#include "InfToPost.h"
//#include "Evaluator.h"
//#include "Parser.h"
//
//int main(int argc, char *argv[])
//try
//{
//	std::vector<std::string> args(argv + 1, argv + argc);
//	bool verbose(false), parse(false);
//
//	std::vector<std::string> tokens, postbuf;
//	std::string in;
//
//	{
//		std::ostringstream oss;
//		for(std::istream_iterator<std::string> i(std::cin), e ; i != e ; ++i) oss << *i << " ";
//		in = oss.str();
//	}
//
//	dav::tokenizer::parse(in.cbegin(), in.cend(), tokens);
//
//	for(const std::string& arg : args)
//	{
//		if(arg == "-p") parse = true;
//		else if(arg == "-v") verbose = true;
//	}
//
//	if(parse)
//	{
//		dav::parser::parse(tokens.cbegin(), tokens.cend(), std::cout);
//	}
//	else
//	{
//		dav::infixtopostfix::parse(tokens.cbegin(), tokens.cend(), postbuf);
//		
//		if(verbose) { for(const auto& s : postbuf) std::cout << s << " "; std::cout << std::endl; }
//
//		double v = dav::evaluator::parse(postbuf.cbegin(), postbuf.cend());
//		std::cout << "= " << Round<4>()(v) << std::endl;
//	}
//
//	return 0;
//}
//catch(const std::string& e)
//{
//	std::cerr << std::endl << "ERR: " << e << std::endl;
//	return 1;
//}

