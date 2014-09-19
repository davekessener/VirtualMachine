#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include "stack.hpp"
#include "lex.h"
#include <dav/Logger.h>
#include <boost/lexical_cast.hpp>

//using namespace dav::lex;
using boost::lexical_cast;

class SVec
{
	public:
	typedef std::vector<std::string> vec_t;
	typedef vec_t::iterator iterator;
	typedef vec_t::const_iterator const_iterator;

	public:
		virtual ~SVec( ) { }
		SVec& operator<<(const std::string& s) { if(!s.empty()) v_.push_back(s); return *this; }
		iterator begin( ) { return v_.begin(); }
		iterator end( ) { return v_.end(); }
		const_iterator begin( ) const { return v_.cbegin(); }
		const_iterator end( ) const { return v_.cend(); }
		const_iterator cbegin( ) const { return v_.cbegin(); }
		const_iterator cend( ) const { return v_.cend(); }
		size_t size( ) const { return v_.size(); }
		bool empty( ) const { return v_.empty(); }
	private:
		vec_t v_;
};

typedef String<'('> OPEN_PARA_S;
typedef String<')'> CLOSE_PARA_S;
typedef String<'+'> ADD_S;
typedef String<'-'> SUB_S;
typedef String<'*'> MUL_S;
typedef String<'/'> DIV_S;
typedef String<'[', '0', '-', '9', ']', '+'> ID_NUM_REGEX;
typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> ID_DEC_REGEX;

typedef String<'[', '0', '-', '9', ']'> ID_DIGIT_REGEX;
typedef String<'.'> DOT_S;
typedef String<'[', 'e', 'E', ']'> ID_EXP_REGEX;

template<int V>
using Translation = Number<V>;

typedef Parser
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
DParser;

// # ===========================================================================

template<typename ST, typename O>
struct Finalizer
{
	static void run(ST& st, O& o)
	{
		o.finalize();
	}
};
typedef Hook<Finalizer> Finalize;

class Printer : public SVec
{
	public:
		Printer& operator<<(const std::string& s) { tmp_.push_back(s); return *this; }
		void finalize( );
	private:
		SVec::vec_t tmp_;
};

void Printer::finalize(void)
{
	std::stringstream ss;
	
	for(const std::string& s : tmp_)
	{
		ss << s;
	}
	
	static_cast<SVec&>(*this) << ss.str();
	
	tmp_.clear();
}

typedef String<'[', '\\', 't', ' ', ']'> ID_WS_REGEX;
typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '_', ']'> ID_IDT_BEG_REGEX;
typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', '_', ']'> ID_IDT_END_REGEX;
typedef String<'[', '\\', '`', '\\', '~', '\\', '!', '\\', '@', '\\', '#', '\\', '$', '\\', '%', '\\', '^', '\\', '&', '\\', '*', '\\', '(', '\\', ')', '\\', '-', '\\', '_', '\\', '=', '\\', '+', '\\', '[', '\\', ']', '\\', '{', '\\', '}', '\\', '\'', '\\', '"', '\\', '\\', '\\', '|', '\\', '/', '\\', '?', '\\', '.', '\\', '>', '\\', ',', '\\', '<', ']'> ID_OP_REGEX;

typedef Parser
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

// # ===========================================================================

typedef double number_t;

template<typename T>
class StackHolderImpl
{
	public:
	typedef dav::Stack<T> stack_t;

	public:
		StackHolderImpl<T>& operator<<(const std::string& s) { s_.push(lexical_cast<T>(s)); return *this; }
		stack_t& get( ) { return s_; }
	private:
		stack_t s_;
};
typedef StackHolderImpl<number_t> StackHolder;

template<typename T>
struct Execute : HookBase
{
	template<typename ST, typename O>
	struct Do
	{
		static void run(ST& st, O& o)
		{
			T::run(o.get());
		}
	};

	template<typename ST, typename O>
	struct Make
	{
		typedef HookImpl<ST, O, Do> type;
	};
};

template<typename S, template<typename> class BOP, typename E = dav::ErrorThrow<>>
struct SecureOP
{
	typedef typename S::value_type value_type;
	typedef BOP<value_type> op_type;
	typedef typename E::error_type error_type;
	enum { operand_count = op_type::operand_count };

	static void run(S& st)
	{
		if(st.size() < operand_count)
		{
			E::OnError("Operation '" + GetName<op_type>::toString()
				+ "' failed, stacksize < " + lexical_cast<std::string>(operand_count)
				+ " (" + lexical_cast<std::string>(st.size()) + ")");
		}

		value_type t1(st.top()); st.pop();
		value_type t2(st.top()); st.pop();
		value_type r(op_type::run(t1, t2));
		st.push(r);
	}
};

template<int C> struct OPN { enum { operand_count = C }; };
template<typename T> struct Addition       : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 + t2; } };
template<typename T> struct Subtraction    : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 - t2; } };
template<typename T> struct Multiplication : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 * t2; } };
template<typename T> struct Division       : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 / t2; } };

typedef SecureOP<StackHolder::stack_t, Addition> Add;
typedef SecureOP<StackHolder::stack_t, Subtraction> Sub;
typedef SecureOP<StackHolder::stack_t, Multiplication> Mul;
typedef SecureOP<StackHolder::stack_t, Division> Div;

typedef PrintID Push;

typedef Parser
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
Executor;

// # ===========================================================================

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	if(args.size() != 2) throw std::string("ERR: Need to supply exactly one term!");

	std::vector<std::string> term(args.at(1).size());
	std::copy(args.at(1).cbegin(), args.at(1).cend(), term.begin());

	Printer p;
	SVec sv;
	StackHolder sh;

	Tokenizer::parse(term.cbegin(), term.cend(), p);
	DParser::parse(p.cbegin(), p.cend(), sv);
	Executor::parse(sv.cbegin(), sv.cend(), sh);

	auto &s(sh.get());
	std::cout << s.size();

	if(s.size() >= 1)
	{
		std::cout << ": " << s.top();
	}

	std::cout << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr<< e << std::endl;
}

