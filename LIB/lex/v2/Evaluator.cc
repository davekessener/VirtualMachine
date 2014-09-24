#include "Evaluator.h"
#include <boost/lexical_cast.hpp>
#include "lex.hpp"
#include "lib.hpp"
#include "Stack.hpp"

#define MXT_E 2.76

using boost::lexical_cast;

namespace dav { namespace evaluator {

	using lex::Analysis;
	using lex::Production;
	using lex::Empty;
	using lex::End;
	using lex::Hook;
	using lex::Literal;
	using lex::Match;
	using lex::StringStore;

	typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> ID_DEC_REGEX;
	typedef String<'+'> ADD_S;
	typedef String<'-'> SUB_S;
	typedef String<'*'> MUL_S;
	typedef String<'/'> DIV_S;
	typedef String<'^'> EXP_S;
	typedef String<'S', 'I', 'N'> SIN_S;
	typedef String<'C', 'O', 'S'> COS_S;
	typedef String<'T', 'A', 'N'> TAN_S;
	typedef String<'L', 'G'> LG_S;
	typedef String<'L', 'N'> LN_S;
	typedef String<'S', 'Q', 'R', 'T'> SQRT_S;

	typedef Stack<number_t> stack_t;

	template<typename T>
	class SingletonWrapper
	{
		public:
		typedef DoDecay<T> type;

		public:
			static type& Instance( ) { static type t; return t; }
		private:
	};

	typedef SingletonWrapper<stack_t> GStack;

	template<int I>
	struct NOP;

	template<>
	struct NOP<0>
	{
		template<typename OP, typename S>
		void run(S& st)
		{
			st.push(OP::run());
		}
	};

	template<>
	struct NOP<1>
	{
		template<typename OP, typename S>
		void run(S& st)
		{
			typedef typename S::value_type value_type;

			value_type v(st.top()); st.pop();

			st.push(OP::run(v));
		}
	};

	template<>
	struct NOP<2>
	{
		template<typename OP, typename S>
		void run(S& st)
		{
			typedef typename S::value_type value_type;

			value_type v1(st.top()); st.pop();
			value_type v2(st.top()); st.pop();

			st.push(OP::run(v1, v2));
		}
	};

	template<typename S, template<typename> class BOP, typename E = ErrorThrow<>>
	struct SecureOP
	{
		typedef typename S::type stack_type;
		typedef typename stack_type::value_type value_type;
		typedef BOP<value_type> op_type;
		typedef typename E::error_type error_type;
		enum { operand_count = op_type::operand_count };
		typedef NOP<operand_count> NOperator;
	
		static void run()
		{
			stack_type &st(S::Instance());
			if(st.size() < operand_count)
			{
				E::OnError("Operation '" + GetName<op_type>::toString()
					+ "' failed, stacksize < " + lexical_cast<std::string>(operand_count)
					+ " (" + lexical_cast<std::string>(st.size()) + ")");
			}

			NOperator::run<op_type>(st);
		}
	};
	
	template<int C> struct OPN { enum { operand_count = C }; };
	template<typename T> struct Addition         : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 + t2; } };
	template<typename T> struct Subtraction      : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 - t2; } };
	template<typename T> struct Multiplication   : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 * t2; } };
	template<typename T> struct Division         : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 / t2; } };
	template<typename T> struct Exponentiation   : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 ^ t2; } };
	template<typename T> struct Sine		     : OPN<1> { static T run(const T& v) { return std::sin(v); } };
	template<typename T> struct Cosine           : OPN<1> { static T run(const T& v) { return std::cos(v); } };
	template<typename T> struct Tangent          : OPN<1> { static T run(const T& v) { return std::tan(v); } };
	template<typename T> struct Logarithm        : OPN<1> { static T run(const T& v) { return std::log(v) / log(MXT_E); } };
	template<typename T> struct NaturalLogarithm : OPN<1> { static T run(const T& v) { return std::log(v); } };
	template<typename T> struct Squareroot       : OPN<1> { static T run(const T& v) { return std::sqrt(v); } };
	
	typedef SecureOP<GStack, Addition> Add;
	typedef SecureOP<GStack, Subtraction> Sub;
	typedef SecureOP<GStack, Multiplication> Mul;
	typedef SecureOP<GStack, Division> Div;
	typedef SecureOP<GStack, Exponentiation> Exp;
	typedef SecureOP<GStack, Sine> Sin;
	typedef SecureOP<GStack, Cosine> Cos;
	typedef SecureOP<GStack, Tangent> Tan;
	typedef SecureOP<GStack, Logarithm> Lg;
	typedef SecureOP<GStack, NaturalLogarithm> Ln;
	typedef SecureOP<GStack, Squareroot> Sqrt;

	struct Push
	{
		static void run(void)
		{
			GStack::Instance().push((boost::lexical_cast<number_t>(StringStore::get())));
		}
	};

	typedef Analysis
	<
		MakeTypeList
		<
			MakeTypeList
			<
				MakeTypeList<Production<1>, End>
			>,
			MakeTypeList
			<
				MakeTypeList<Production<3>, Production<2>>
			>,
			MakeTypeList
			<
				MakeTypeList<Production<3>, Production<2>>,
				MakeTypeList<Empty>
			>,
			MakeTypeList // 3
			<
				MakeTypeList<Match<ID_DEC_REGEX>, Hook<Push>>,
				MakeTypeList<Literal<ADD_S>, Hook<Add>>,
				MakeTypeList<Literal<SUB_S>, Hook<Sub>>,
				MakeTypeList<Literal<MUL_S>, Hook<Mul>>,
				MakeTypeList<Literal<DIV_S>, Hook<Div>>,
				MakeTypeList<Literal<EXP_S>, Hook<Exp>>,
				MakeTypeList<Literal<SIN_S>, Hook<Sin>>,
				MakeTypeList<Literal<COS_S>, Hook<Cos>>,
				MakeTypeList<Literal<TAN_S>, Hook<Tan>>,
				MakeTypeList<Literal<LG_S>, Hook<Lg>>,
				MakeTypeList<Literal<LN_S>, Hook<Ln>>,
				MakeTypeList<Literal<SQRT_S>, Hook<Sqrt>>
			>
		>
	>
	Evaluator;

	number_t parse(io::Reader& in)
	{
		stack_t &st(GStack::Instance());

		Evaluator::parse(in);

		if(st.size() != 1) throw aux::stringify("expected stack-size to be 1 instead of ", st.size(), " after execution!");

		number_t v(st.top()); st.pop();

		return v;
	}
}}


