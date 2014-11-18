#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include "Tokenizer.h"
#include "InfToPost.h"
#include "Evaluator.h"
#include "Parser.h"

#define MXT_ROUND 4

using dav::evaluator::number_t;

void eval(bool = false);
void parse( );
void param(const std::string&, bool);

template<typename I, typename O> void tokenize(I&&, O&&);
template<typename I, typename O> void convert(I&&, O&&);
template<typename I> number_t evaluate(I&&);
std::ostream& operator<<(std::ostream&, const std::vector<std::string>&);

// # ==========================================================================

int main(int argc, char *argv[])
{
	typedef std::function<void(void)> exec_fn;

	std::vector<std::string> args(argv + 1, argv + argc);
	std::map<std::string, exec_fn> modes;
	std::string input("");
	bool verbose = false;
	exec_fn run = [&verbose, &input]() { param(input, verbose); };

	modes["-p"] = [&run]() { run = &parse; };
	modes["-v"] = [&verbose]() { verbose = true; };
	modes["-i"] = [&run, &verbose]() { run = [&verbose]() { eval(verbose); }; };

	for(const std::string& arg : args)
	{
		if(modes.count(arg)) modes.at(arg)();
		else input += arg + " ";
	}

	run();

	return 0;
}

// # --------------------------------------------------------------------------

void eval(bool verbose)
{
	dav::Round<MXT_ROUND> round;

	while(true)
	{
		try
		{
			std::vector<std::string> tokens, postfix;
			std::string in("");
			number_t result;

			std::getline(std::cin, in);

			if(std::cin.eof()) break;
			if(in.empty()) continue;
			if(in.at(0) == '#') { if(verbose) std::cout << in << std::endl; continue; }

			if(verbose) std::cout << in << std::endl;

			tokenize(in, tokens);

			std::cout << tokens << std::endl;

			convert(tokens, postfix);

			std::cout << postfix << std::endl;

			result = evaluate(postfix);

			std::cout << "= " << round(result) << std::endl;
		}
		catch(const std::string& e)
		{
			std::cerr << e << std::endl;
		}
	}
}

// # ---------------------------------------------------------------------------

void param(const std::string& expr, bool v)
{
	dav::Round<MXT_ROUND> round;

	try
	{
		std::vector<std::string> tokens, postfix;
		number_t result;

		if(expr.empty()) throw std::string("ERR: No input!");

		tokenize(expr, tokens);

		std::cout << tokens << std::endl;

		convert(tokens, postfix);

		if(v) std::cout << postfix << std::endl;

		result = evaluate(postfix);

		std::cout << "= " << round(result) << std::endl;
	}
	catch(const std::string& e)
	{
		std::cerr << e << std::endl;
	}
}

// # ---------------------------------------------------------------------------

inline std::string drainStdin( ) { std::ostringstream oss; for(std::istream_iterator<std::string> i(std::cin), e ; i != e ; ++i) oss << *i << " "; return oss.str(); }

void parse(void)
{
	std::string input(drainStdin());
	std::vector<std::string> tokens;

	try
	{
		tokenize(input, tokens);
		dav::parser::parse(tokens, std::cout);
	}
	catch(const std::string& e)
	{
		std::cerr << "Parser error: " << e << std::endl;
	}
}

// # ==========================================================================

template<typename I, typename O>
void tokenize(I&& in, O&& out)
try
{
	dav::tokenizer::parse(in, out);
}
catch(const std::string& e)
{
	throw std::string("Tokenizer error: " + e);
}

template<typename I, typename O>
void convert(I&& in, O&& out)
try
{
	dav::infixtopostfix::parse(in, out);
}
catch(const std::string& e)
{
	throw std::string("Infix to Postfix conversion error: " + e);
}

template<typename I>
number_t evaluate(I&& in)
try
{
	return dav::evaluator::parse(in);
}
catch(const std::string& e)
{
	throw std::string("Postfix evaluation error: " + e);
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v)
{
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<std::string>(os, " "));
	
	return os;
}

