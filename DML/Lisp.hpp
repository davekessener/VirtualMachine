#ifndef DML_LISP_H
#define DML_LISP_H

#include "DML.h"

namespace lisp
{
	typedef decltype(nullptr) NIL;

	template<typename T, typename TT>
	struct Cons
	{
		typedef T HEAD;
		typedef TT TAIL;
	};

	template<typename T, typename ... TT>
	struct List
	{
		typedef Cons<T, typename List<TT...>::RET> RET;
	};

	template<typename T>
	struct List<T>
	{
		typedef Cons<T, NIL> RET;
	};

	template<typename T>
	struct Eval
	{
		typedef typename T::HEAD::template APPLY<typename T::TAIL>::RET RET;
	};

	template<typename T>
	struct Car
	{
		typedef typename T::HEAD RET;
	};

	template<typename T>
	struct Cdr
	{
		typedef typename T::TAIL RET;
	};

	template<typename T, long L = 0>
	struct Length
	{
		struct RET { enum { VAL = Length<typename T::TAIL, L + 1>::VAL }; };
		enum { VAL = RET::VAL };
	};

	template<long L>
	struct Length<NIL, L>
	{
		struct RET { enum { VAL = L }; };
		enum { VAL = RET::VAL };
	};

	template<typename L, typename T>
	struct Append
	{
		typedef Cons<typename L::HEAD, typename Append<typename L::TAIL, T>::RET> RET;
	};

	template<typename T>
	struct Append<NIL, T>
	{
		typedef Cons<T, NIL> RET;
	};

	template<typename T>
	struct Empty
	{
		struct RET { enum { VAL = 0 }; };
		enum { VAL = RET::VAL };
	};

	template<>
	struct Empty<NIL>
	{
		struct RET { enum { VAL = 1 }; };
		enum { VAL = RET::VAL };
	};

	template<long N, long D>
	struct Number
	{
		typedef Number<N, D> RET;
		enum { VAL = N, NUM = N, DEN = D };
	};

	template<long N>
	struct Int
	{
		typedef typename Number<N, 1>::RET RET;
		enum { VAL = RET::VAL };
	};

	namespace
	{
		template<bool C, typename T1, typename T2>
		struct _if;

		template<typename T1, typename T2>
		struct _if<true, T1, T2>
		{
			typedef typename T1::RET RET;
		};

		template<typename T1, typename T2>
		struct _if<false, T1, T2>
		{
			typedef typename T2::RET RET;
		};
	}

	template<typename C, typename T1, typename T2>
	struct If
	{
		typedef typename _if<C::VAL, T1, T2>::RET RET;
	};

	namespace
	{
		template<long N1, long N2>
		struct _gt
		{
			struct RET { enum { VAL = N1 > N2 }; };
			enum { VAL = RET::VAL };
		};

		template<long N1, long N2>
		struct _lt
		{
			struct RET { enum { VAL = N1 < N2 }; };
			enum { VAL = RET::VAL };
		};

		template<long N1, long N2>
		struct _ge
		{
			struct RET { enum { VAL = N1 >= N2 }; };
			enum { VAL = RET::VAL };
		};

		template<long N1, long N2>
		struct _le
		{
			struct RET { enum { VAL = N1 <= N2 }; };
			enum { VAL = RET::VAL };
		};

		template<long N1, long N2>
		struct _eq
		{
			struct RET { enum { VAL = N1 == N2 }; };
			enum { VAL = RET::VAL };
		};

		template<long N1, long N2>
		struct _ne
		{
			struct RET { enum { VAL = N1 != N2 }; };
			enum { VAL = RET::VAL };
		};
	}

	template<long I>
	struct Bool
	{
		struct RET { enum { VAL = I ? 1 : 0 }; };
		enum { VAL = RET::VAL };
	};

	template<typename T>
	struct Not
	{
		struct RET { enum { VAL = T::RET::VAL ? 0 : 1 }; };
		enum { VAL = RET::VAL };
	};

	namespace
	{
		template<long N1, long N2>
		struct _gcd
		{
			static_assert(N2 > 0 && N1 > 0, "Cannot be negative or zero!");

			template<long NN1, long NN2>
			struct Let
			{
				enum { NN3 = NN1 % NN2 };
				typedef typename If<Not<Bool<NN3>>, Int<NN2>, _gcd<NN2, NN3>>::RET RET;
			};

			typedef typename If<_lt<N1, N2>, _gcd<N2, N1>, Let<N1, N2>>::RET RET;
		};
	}

	template<typename N>
	struct Neg
	{
		typedef typename Number<-N::RET::NUM, N::RET::DEN>::RET RET;
	};

	template<typename N>
	struct Inv
	{
		typedef typename Number<N::RET::DEN, N::RET::NUM>::RET RET;
	};

	template<typename N1, typename N2>
	struct Add
	{
		enum { 	NN1 = N1::RET::NUM * N2::RET::DEN,
				NN2 = N2::RET::NUM * N1::RET::DEN,
				NN  = NN1 + NN2,
				ND  = N1::RET::DEN * N2::RET::DEN,
				GCD = _gcd<NN, ND>::RET::NUM };
		typedef typename Number<NN / GCD, ND / GCD>::RET RET;
	};

	template<typename N1, typename N2>
	struct Sub
	{
		typedef typename Add<N1, typename Neg<N2>::RET>::RET RET;
	};

	template<typename N1, typename N2>
	struct Mul
	{
		enum {	N   = N1::RET::NUM * N2::RET::NUM,
				D   = N1::RET::DEN * N2::RET::DEN,
				GCD = _gcd<N, D>::RET::NUM };
		typedef typename Number<N / GCD, D / GCD>::RET RET;
	};

	template<typename N1, typename N2>
	struct Div
	{
		typedef typename Mul<N1, typename Inv<N2>::RET>::RET RET;
	};

	template<typename N>
	struct Inc
	{
		typedef typename Add<N, Int<1>>::RET RET;
		enum { VAL = RET::VAL };
	};

	template<typename N>
	struct Dec
	{
		typedef typename Sub<N, Int<1>>::RET RET;
		enum { VAL = RET::VAL };
	};

//	(define
//		'heron
//		(lambda
//			(a n)
//			(if
//				(== n 1)
//				1
//				(let
//					((h (heron a (- n 1))))
//					(/
//						(+ h (/ a h))
//						2)))))
	
	template<typename A, typename N>
	struct Heron
	{
		typedef typename Heron<A, typename Dec<N>::RET>::RET H;
		typedef typename Div<Add<H, Div<A, H>>, Int<2>>::RET RET;
	};

	template<typename A>
	struct Heron<A, Number<1, 1>>
	{
		typedef typename Int<1>::RET RET;
	};
}

#endif

