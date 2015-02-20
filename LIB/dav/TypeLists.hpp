#ifndef DAV_LIB_TYPELISTS_H
#define DAV_LIB_TYPELISTS_H

#include "inc.h"

namespace dav
{
	namespace lib
	{
		namespace type_list
		{
			template<typename H, typename ... T>
			struct TypeList
			{
				using head = H;
				using tail = TypeList<T...>;
			};

			template<typename H, typename ... T>
			struct TypeList<H, TypeList<T...>>
			{
				using head = H;
				using tail = TypeList<T...>;
			};

			template<>
			struct TypeList<>
			{
			};

			template<typename>
			struct Length;

			template<typename H, typename ... T>
			struct Length<TypeList<H, T...>>
			{
				static const size_t value = 1 + Length<TypeList<T...>>::value;
			};

			template<>
			struct Length<TypeList<>>
			{
				static const size_t value = 0;
			};

			template<typename>
			struct Reverse;

			template<typename H, typename T, typename ... TT>
			struct Reverse<TypeList<H, T, TT...>>
			{
				using type = TypeList<typename Reverse<TypeList<T, TT...>>::type, H>;
			};

			template<typename T>
			struct Reverse<TypeList<T>>
			{
				using type = TypeList<T>;
			};

			template<size_t, typename>
			struct Get;

			template<size_t P, typename H, typename ... T>
			struct Get<P, TypeList<H, T...>>
			{
				using type = typename Get<P - 1, TypeList<T...>>::type;
			};

			template<typename H, typename ... T>
			struct Get<0, TypeList<H, T...>>
			{
				using type = H;
			};

			template<size_t P>
			struct Get<P, TypeList<>>
			{
				static_assert(false, "ERR: Trying to access element after list-end!");
			};

			template<size_t, typename, typename>
			struct Set;

			template<size_t P, typename V, typename H, typename ... T>
			struct Set<P, V, TypeList<H, T...>>
			{
				using type = TypeList<H, typename Set<P - 1, V, TypeList<T...>>::type>;
			};

			template<typename V, typename H, typename ... T>
			struct Set<0, V, TypeList<H, T...>>
			{
				using type = TypeList<V, T...>;
			};

			template<size_t P, typename V>
			struct Set<P, V, TypeList<>>
			{
				static_assert(false, "ERR: Trying to overwrite element after list-end!");
			};

			template<template <typename>, typename>
			struct Apply;

			template<template <typename> F, typename H, typename ... T>
			struct Apply<F, TypeList<H, T...>>
			{
				using type = TypeList<typename F<H>::type, typename Apply<F, TypeList<T...>>::type>;
			};

			template<template <typename> F>
			struct Apply<F, TypeList<>>
			{
				using type = TypeList<>;
			};

			template<typename L, typename V, typename R>
			struct Iterator
			{
				using left = L;
				using right = R;
				using type = V;

				using next = Iterator<TypeList<V, L>, typename R::head, typename R::tail>;
				using prev = Iterator<typename L::tail, typename L::head, TypeList<V, R>>;
			};

			template<typename H, typename ... T>
			using Begin = Iterator<TypeList<>, H, TypeList<T...>>;

			template<typename H, typename ... T>
			using End = Begin<typename Reverse<TypeList<H, T...>>::type>;

// # ===========================================================================

			template<unsigned V, typename T, typename F>
			struct IfImpl
			{
				using type = typename T::type;
			}

			template<typename T, typename F>
			struct IfImpl<0, T, F>
			{
				using type = typename F::type;
			};

			template<typename C, typename T, typename F>
			struct If
			{
				using type = typename IfImpl<C::value, T, F>::type;
			};

			template<typename C, typename T, typename F>
			using DoIf = typename If<C, T, F>::type;

			using NIL = decltype(nullptr);

			template<BYTE V, typename T = NIL>
			struct BigInt
			{
				static const BYTE value = V;
				using type = BigInt<V, T>;
				using tail = T;
			};

			template<typename>
			struct QSum;

			template<BYTE V, typename T>
			struct QSum<BigInt<V, T>>
			{
				static const QWORD value = V + QSum<T>::value;
			};

			template<>
			struct QSum<NIL>
			{
				static const QWORD value = 0;
			};

			template<BYTE V, typename TT, typename T, typename F>
			struct If<BigInt<V, TT>, T, F>
			{
				using type = typename IfImpl<QSum<BigInt<V, TT>>::value, T, F>::type;
			};

			template<typename, BYTE>
			struct Append;

			template<BYTE V, typename T, BYTE A>
			struct Append<BigInt<V, T>, A>
			{
				using type = BigInt<V, typename Append<T, A>::type>;
			};

			template<BYTE A>
			struct Append<NIL, A>
			{
				using type = BigInt<A>;
			};

			template<BYTE V, typename T>
			struct Reverse<BigInt<V, T>>
			{
				using type = typename Append<typename Reverse<T>::type, V>::type;
			};

			template<BYTE V>
			struct Reverse<BigInt<V, NIL>>
			{
				using type = BigInt<V, NIL>;
			};

			template<typename T>
			using DoReverse = typename Reverse<T>::type;

			template<typename, typename, BYTE = 0>
			struct AddImpl;

			template<BYTE V1, typename T1, BYTE V2, typename T2, BYTE A>
			struct AddImpl<BigInt<V1, T1>, BigInt<V2, T2>, A>
			{
				static const BYTE value = V1 + V2 + A;
				using type = BigInt<value % 10, typename AddImpl<T1, T2, value / 10>::type>;
			};

			template<BYTE A>
			struct AddImpl<NIL, NIL, A>
			{
				using type = BigInt<A>;
			};

			template<>
			struct AddImpl<NIL, NIL, 0>
			{
				using type = NIL;
			};

			template<typename T, typename ... TT>
			struct Add
			{
				using type = DoReverse<typename AddImpl<DoReverse<T>, DoReverse<typename Add<TT...>::type>>::type>;
			};

			template<>
			struct Add<>
			{
				using type = BigInt<0>;
			};

			template<typename ... T>
			using DoAdd = typename Add<T...>::type;

			template<BYTE V, typename T>
			struct Length<BigInt<V, T>>
			{
				static const QWORD value = 1 + Length<T>::value;
			};

			template<>
			struct Length<NIL>
			{
				static const QWORD value = 0;
			};

			template<int V>
			struct IntWrapper
			{
				static const int value = V;
			};

			template<typename T1, typename T2> struct GT { static const QWORD value = T1::value > T2::value; }
			
			template<typename T, typename ... TT>
			struct OR
			{	
				static const QWORD value = T::value || OR<TT...>::value;
			};

			template<>
			struct OR<>
			{
				static const QWORD value = 0;
			}
			
			template<typename T, typename ... TT>
			struct AND
			{	
				static const QWORD value = T::value && AND<TT...>::value;
			};

			template<>
			struct AND<>
			{
				static const QWORD value = 1;
			}

			template<typename T> struct NOT { static const QWORD value = !T::value; }

			template<typename T1, typename T2>
			struct GreaterThanImpl
			{
				static const QWORD value = 0;
			}

			template<BYTE V1, typename T1, BYTE V2, typename T2>
			struct GreaterThanImpl<BigInt<V1, T1>, BigInt<V2, T2>>
			{
				static const QWORD value = V1 > V2 || GreaterThanImpl<T1, T2>::value;
			};

			template<typename, typename>
			struct GreaterThan;

			template<BYTE V1, typename T1, BYTE V2, typename T2>
			struct GreaterThan<BigInt<V1, T1>, BigInt<V2, T2>>
			{
				using I1 = BigInt<V1, T1>;
				using I2 = BigInt<V2, T2>;
				static const QWORD L1 = Length<I1>::value;
				static const QWORD L2 = Length<I2>::value;
				static const QWORD value = L1 > L2 || (L1 == L2 && GreaterThanImpl<I1, I2>::value);
			};

			template<typename T1, typename T2> struct Equals { static const QWORD value = !GreaterThan<T1, T2>::value && !GreaterThan<T2, T1>::value; };
			template<typename T1, typename T2> struct NotEquals { static const QWORD value = GreaterThan<T1, T2>::value || GreaterThan<T2, T1>::value; };
			template<typename T1, typename T2> struct LessThan { static const QWORD value = GreaterThan<T2, T1>::value; };
			template<typename T1, typename T2> struct GreaterOrEqualThan { static const QWORD value = !GreaterThan<T2, T1>::value; };
			template<typename T1, typename T2> struct LessOrEqualThan { static const QWORD value = !GreaterThan<T1, T2>::value; };

			template<typename, typename, BYTE = 0>
			struct SubImpl;

			template<BYTE V1, typename T1, BYTE V2, typename T2, BYTE A>
			struct SubImpl<BigInt<V1, T1>, BigInt<V2, T2>, A>
			{
				static const BYTE carry = V1 < V2 + A;
				static const BYTE V = (carry ? 10 : 0) + V1 - V2 - A;
				using type = BigInt<V, typename SubImpl<T1, T2, carry>::type>;
			};

			template<BYTE V, typename T, BYTE A>
			struct SubImpl<BigInt<V, T>, NIL, A>
			{
				static const BYTE C = V < A;
				using type = BigInt<(C ? 10 : 0) + V - A, typename SubImpl<T, NIL, C>::type>;
			};

			template<>
			struct SubImpl<NIL, NIL, 0>
			{
				using type = NIL;
			};

			template<typename, typename>
			struct Sub;

			template<BYTE V1, typename T1, BYTE V2, typename T2>
			struct Sub<BigInt<V1, T2>, BigInt<V2, T2>>
			{
				using I1 = BigInt<V1, T1>;
				using I2 = BigInt<V2, T2>;

				using type = DoIf
							 <
								GreaterThan<I2, I1>,
								Sub<I2, I1>,
								DoIf
								<
									Equals<I1, I2>,
									BigInt<0>,
									DoReverse<typename SubImpl<DoReverse<I1>, DoReverse<I2>>::type>
								>
							 >;
			};

			template<typename T1, typename T2>
			using DoSub = typename Sub<T1, T2>::type;

			template<BYTE, typename, QWORD>
			struct BigNum;

			template<BYTE S, QWORD E, BYTE V, typename T>
			struct BigNum<S, BigInt<V, T>, E>
			{
				static const BYTE sign = S;
				static const QWORD exp = E;
				using mantissa = BigInt<V, T>;
				using type = BigNum<sign, mantissa, exp>;
			};
		}
	}
}

#endif

