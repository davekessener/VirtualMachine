#include "Evaluator.h"
#include <boost/lexical_cast.hpp>
#include "lex.hpp"
#include "lib.hpp"
#include "Stack.hpp"

#define MXT_PI   3.14159265358979323846264338327950288419716939937510
#define MXT_E    2.71828182845904523536028747135266249775724709369995
#define MXT_LN10 2.30258509299404568401799145468436420760110148862877

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
	typedef String<'N', 'E', 'G'> NEG_S;
	typedef String<'E'> E_S;
	typedef String<'P', 'I'> PI_S;

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
		template<typename S, typename OP>
		static void run(S& st, OP&& op)
		{
			st.push(op());
		}
	};

	template<>
	struct NOP<1>
	{
		template<typename S, typename OP>
		static void run(S& st, OP&& op)
		{
			typedef typename S::value_type value_type;

			value_type v(st.top()); st.pop();

			st.push(op(v));
		}
	};

	template<>
	struct NOP<2>
	{
		template<typename S, typename OP>
		static void run(S& st, OP&& op)
		{
			typedef typename S::value_type value_type;

			value_type v1(st.top()); st.pop();
			value_type v2(st.top()); st.pop();

			st.push(op(v2, v1));
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

			NOperator::run(st, op_type());
		}
	};
	
	template<int C> struct OPN { enum { operand_count = C }; };

	template<typename T> struct Addition         : OPN<2> 
	{
		constexpr T operator()(const T& t1, const T& t2) const { return t1 + t2; }
	};

	template<typename T> struct Subtraction      : OPN<2> 
	{
		 constexpr T operator()(const T& t1, const T& t2) const { return t1 - t2; } 
	};

	template<typename T> struct Multiplication   : OPN<2> 
	{
		 constexpr T operator()(const T& t1, const T& t2) const { return t1 * t2; } 
	};

	template<typename T> struct Division         : OPN<2> 
	{
		 constexpr T operator()(const T& t1, const T& t2) const { return t1 / t2; } 
	};

	template<typename T> struct Exponentiation   : OPN<2> 
	{
		 constexpr T operator()(const T& t1, const T& t2) const { return std::pow(t1, t2); } 
	};

	template<typename T> struct Sine		     : OPN<1> 
	{
		 T operator()(const T& v) const { return std::sin(v); } 
	};

	template<typename T> struct Cosine           : OPN<1> 
	{
		 T operator()(const T& v) const { return std::cos(v); } 
	};

	template<typename T> struct Tangent          : OPN<1> 
	{
		 T operator()(const T& v) const { return std::tan(v); } 
	};

	template<typename T> struct Logarithm        : OPN<1> 
	{
		 T operator()(const T& v) const { return std::log(v) / MXT_LN10; } 
	};

	template<typename T> struct NaturalLogarithm : OPN<1> 
	{
		 T operator()(const T& v) const { return std::log(v); } 
	};

	template<typename T> struct Squareroot       : OPN<1> 
	{
		 T operator()(const T& v) const { return std::sqrt(v); } 
	};

	template<typename T> struct Negation         : OPN<1> 
	{
		 constexpr T operator()(const T& v) const { return -v; } 
	};

	
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
	typedef SecureOP<GStack, Negation> Neg;

	struct Push
	{
		static void run(void)
		{
			GStack::Instance().push((boost::lexical_cast<number_t>(StringStore::get())));
		}
	};

	struct PushE  { static void run() { GStack::Instance().push(MXT_E);  } };
	struct PushPI { static void run() { GStack::Instance().push(MXT_PI); } };

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
				MakeTypeList<Literal<E_S>, Hook<PushE>>,
				MakeTypeList<Literal<PI_S>, Hook<PushPI>>,
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
				MakeTypeList<Literal<SQRT_S>, Hook<Sqrt>>,
				MakeTypeList<Literal<NEG_S>, Hook<Neg>>
			>
		>
	>
	Evaluator;

	number_t parse(io::Reader& in)
	{
		stack_t &st(GStack::Instance());

		Evaluator::parse(in);

		if(st.size() != 1) 
			throw aux::stringify
			(
				"expected stack-size to be 1 instead of ",
				st.size(),
				" after execution!"
			);

		number_t v(st.top()); st.pop();

		return v;
	}
}}


