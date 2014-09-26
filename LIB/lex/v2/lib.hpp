#ifndef DAV_LIB_H
#define DAV_LIB_H

#include <iosfwd>
#include <iterator>
#include <string>
#include <type_traits>

namespace dav
{
	template<typename T>
	struct Identity
	{
		typedef T type;
	};

	template<int I, typename T = void>
	struct EnableIf
	{
		typedef T type;
	};

	template<typename T>
	struct EnableIf<0, T>
	{
	};

	template<typename I, typename T = void>
	using DoEnableIf = typename EnableIf<I::value, T>::type;

	template<typename T>
	struct HasName
	{
		struct A { char v[1]; };
		struct B { char v[2]; };

		template<typename C> static constexpr A eval(decltype(&C::toString));
		template<typename C> static constexpr B eval(...);

		enum { value = sizeof(eval<T>(nullptr)) == sizeof(A) };
	};

	template<typename T, int I>
	struct GetNameImpl
	{
		static std::string toString( ) { return "#NONAME#"; }
	};

	template<typename T>
	struct GetNameImpl<T, 1>
	{
		static std::string toString( ) { return T::toString(); }
	};

	template<typename T>
	struct GetName
	{
		static std::string toString( ) { return GetNameImpl<T, HasName<T>::value>::toString(); }
	};

	struct StringBase { };

	template<char ... C>
	struct String : StringBase
	{
		static std::string toString( ) { return std::string{ { C... } }; }
	};

	template<typename T, typename TT>
	struct IsDerived
	{
		typedef typename std::decay<T>::type base_type;
		typedef typename std::decay<TT>::type derived_type;
		struct A { char v[1]; };
		struct B { char v[2]; };
		static constexpr A eval(base_type*);
		static constexpr B eval(...);
		enum { value = sizeof(eval(static_cast<derived_type *>(nullptr))) == sizeof(A) };
	};

	template<typename Base, typename Derived>
	inline constexpr bool DoIsDerived( ) { return IsDerived<Base, Derived>::value; }

	template<typename T>
	using DoDecay = typename std::decay<T>::type;

	template<typename I>
	struct deref_traits
	{
		I i;
		typedef decltype(*i) type;
	};

	template<typename T1, typename T2>
	struct IsEqual
	{
		enum { value = 0 };
	};

	template<typename T>
	struct IsEqual<T, T>
	{
		enum { value = 1 };
	};

	template<typename T>
	using IsString = IsEqual<std::string, typename std::decay<T>::type>;

	template<typename T>
	using DereferencesToString = IsString<typename deref_traits<T>::type>;

	template<int C, typename T1, typename T2>
	struct IfImpl;

	template<typename T1, typename T2>
	struct IfImpl<1, T1, T2>
	{
		typedef typename T1::type type;
	};

	template<typename T1, typename T2>
	struct IfImpl<0, T1, T2>
	{
		typedef typename T2::type type;
	};

	template<typename C, typename T1, typename T2>
	struct If
	{
		typedef typename IfImpl<C::value, T1, T2>::type type;
	};

	struct Num { };

	template<int V>
	struct Number : Num
	{
		enum { value = V };
	};

	template<int I, typename T = double>
	struct Round
	{
		typedef T value_type;
		enum { displacement = 10 * Round<I - 1, T>::displacement };
	
		value_type operator()(const value_type& v) const
		{
			return static_cast<value_type>(static_cast<long>(v * displacement + (v < 0 ? -0.5 : 0.5))) / displacement;
		}
	};
	
	template<typename T>
	struct Round<0, T>
	{
		enum { displacement = 1 };
	};

	template<typename T>
	struct IsIterable
	{
		struct A { char v[1]; };
		struct B { char v[2]; };

		template<typename C> static constexpr A eval(typename C::iterator *);
		template<typename C> static constexpr B eval(...);

		enum { value = sizeof(eval<T>(nullptr)) == sizeof(A) };
	};

//	template<typename T>
//		typename std::enable_if<IsIterable<T>::value>::type
//			operator<<(std::ostream& os, const T& t)
//	{
//		std::transform
//		(
//			t.cbegin(),
//			t.cend(),
//			std::ostream_iterator<std::string>(std::cout, " "),
//			[](const typename T::value_type& v) 
//				{ return boost::lexical_cast<std::string>(v); }
//		);
//	}

// #========================================================================

	typedef decltype(nullptr) NIL;

	template<typename H, typename T>
	struct TypeList
	{
		typedef H head;
		typedef T tail;
	};

	template<typename T, int I>
	struct Get
	{
		static_assert(!IsEqual<T, NIL>::value, "ERR: out of range!");
		typedef typename Get<typename T::tail, I - 1>::type type;
	};

	template<typename T>
	struct Get<T, 0>
	{
		static_assert(!IsEqual<T, NIL>::value, "ERR: out of range!");
		typedef typename T::head type;
	};

	template<typename ... T>
	struct MakeTL;

	template<typename H, typename T, typename ... TT>
	struct MakeTL<H, T, TT...>
	{
		typedef typename MakeTL<T, TT...>::type tail;
		typedef TypeList<H, tail> type;
	};

	template<typename T>
	struct MakeTL<T>
	{
		typedef TypeList<T, NIL> type;
	};

	template<typename ... T>
	using MakeTypeList = typename MakeTL<T...>::type;

	template<typename T, template<typename> class TT>
	struct Transform;

	template<typename H, typename T, template<typename> class F>
	struct Transform<TypeList<H, T>, F>
	{
		typedef TypeList<typename F<H>::type, typename Transform<T, F>::type> type;
	};

	template<template<typename> class F>
	struct Transform<NIL, F>
	{
		typedef NIL type;
	};

	template<typename T, template<typename> class F>
	struct RecursiveTransform
	{
		typedef typename F<T>::type type;
	};

	template<typename H, typename T, template<typename> class F>
	struct RecursiveTransform<TypeList<H, T>, F>
	{
		typedef TypeList<typename RecursiveTransform<H, F>::type, typename RecursiveTransform<T, F>::type> type;
	};

	template<template<typename> class F>
	struct RecursiveTransform<NIL, F>
	{
		typedef NIL type;
	};

	template<int V, typename T, typename TT, template<typename, typename> class F>
	struct IsInImpl;

	template<typename T, typename TT, template<typename, typename> class F>
	struct IsInImpl<1, T, TT, F>
	{
		enum { value = 1 };
	};

	template<typename H, typename T, typename TT, template<typename, typename> class F>
	struct IsInImpl<0, TypeList<H, T>, TT, F>
	{
		enum { value = IsInImpl<F<H, TT>::value, T, TT, F>::value };
	};

	template<typename TT, template<typename, typename> class F>
	struct IsInImpl<0, NIL, TT, F>
	{
		enum { value = 0 };
	};

	template<typename T, typename TT, template<typename, typename> class F = IsEqual>
	struct IsIn
	{
		enum { value = IsInImpl<0, T, TT, F>::value };
	};

// # ===========================================================================


// # ===========================================================================

}

#endif

