#ifndef META_NUMERIC_H
#define META_NUMERIC_H

#include "DML.h"

namespace lisp
{
	namespace numeral
	{
		typedef long long B;
//		static const size_t sizeof(B) N;

// # ---------------------------------------------------------------------------

		template<B V>
		struct Int
		{
			static const B VAL = V;
		};

// # ---------------------------------------------------------------------------

		template<B V, typename T>
		struct BL;

		template<B V1, B V2, typename T>
		struct BL<V1, BL<V2, T>>
		{
			static const B HEAD = V1;
			typedef BL<V2, T> TAIL;
		};

		template<B V>
		struct BL<V, NIL>
		{
			static const B HEAD = V;
			typedef NIL TAIL;
		};

		template<typename T>
		struct Length
		{
			static const size_t VAL = 1 + Length<typename T::TAIL>::VAL;
		};

		template<>
		struct Length<NIL>
		{
			static const size_t VAL = 0;
		};

// # ---------------------------------------------------------------------------
	
		template<typename T1, typename T2 = NIL>
		struct ReverseBL
		{
			typedef typename ReverseBL<typename T1::TAIL, BL<T1::HEAD, T2>>::RET RET;
		};

		template<typename T>
		struct ReverseBL<NIL, T>
		{
			typedef T RET;
		};

// # ---------------------------------------------------------------------------

		template<bool F, typename T1, typename T2>
		struct EvalIf;

		template<typename T1, typename T2>
		struct EvalIf<true, T1, T2>
		{
			typedef typename T1::RET RET;
		};

		template<typename T1, typename T2>
		struct EvalIf<false, T1, T2>
		{
			typedef typename T2::RET RET;
		};

// # ---------------------------------------------------------------------------

		template<typename T1, typename T2, B CIN = 0>
		struct AddBLI
		{
			static_assert(CIN >= 0 && CIN <= 1, "ERR: Not a valid carry!");

			static const B V = T1::HEAD + T2::HEAD + CIN;
			static const B COUT = V < T1::HEAD;
		};

		template<typename T>
		struct AddBLI<T, NIL>
		{
			
		};

		template<typename T>
		struct AddBLI<NIL, T>
		{
			static_assert(sizeof(typename T::__INVALID__), "ERR: Empty list has to be last!");
		};
		
		template<typename T1, typename T2>
		struct AddBL
		{
			typedef typename ReverseBL<T1>::RET RT1;
			typedef typename ReverseBL<T2>::RET RT2;
			typedef typename ReverseBL
			<
				typename EvalIf
				<
					Length<T1>::VAL >= Length<T2>::VAL, 
					AddBLI<RT1, RT2>, 
					AddBLI<RT2, RT1>
				>::RET
			>::RET
			RET;
		};
	}
}

#endif

