#ifndef DAV_LEX_H
#define DAV_LEX_H

#include <type_traits>
#include <string>
#include <boost/lexical_cast.hpp>

#ifdef DEBUG_LOG
#	include <dav/Logger.h>
#	define MXT_LEX_LOG(...) LOG(__VA_ARGS__)
#else
#	define MXT_LEX_LOG(...)
#endif

using boost::lexical_cast;

#ifdef STD_REGEX
#	include <regex>
#	define MXT_REGEX_SCOPE std
#else
#	include <boost/regex.hpp>
#	define MXT_REGEX_SCOPE boost
#endif

namespace dav
{
	template<typename T>
	struct Identity
	{
		typedef T type;
	};

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
		struct A { char v[1]; };
		struct B { char v[2]; };
		static constexpr A eval(T*) { return A(); }
		static constexpr B eval(...) { return B(); }
		enum { value = sizeof(eval(static_cast<TT *>(nullptr))) == sizeof(A) };
	};

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

	namespace lex
	{

// # ===========================================================================

#define MXT_DEREF_CHECK(I) static_assert(DereferencesToString<I>::value, "ERR: Parser operates on std::strings only!")
#define MXT_STRING_CHECK(S) static_assert(HasName<S>::value, "ERR: S needs to be derived from StringBase!")
#define MXT_LOGC (i1==i2?"#EOF#":i1->data())

		typedef String<'*'> ID_DEF_S;
		typedef String<'+'> LIT_DEF_S;
		typedef String<'.', '*'> ID_ALL_S;

		struct FAIL { };

		template<typename SymTable, typename Out>
		struct EndImpl
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);
				if(i1 != i2) throw std::string("ERR: Trailing characters @'" + *i1 + "'");
				return true;
			}

			typedef EndImpl<SymTable, Out> type;
		};

		template<typename SymTable, typename Out>
		struct EmptyImpl
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);
				return true;
			}

			typedef EmptyImpl<SymTable, Out> type;
		};

		template<typename SymTable, typename Out, typename R, typename S, bool Literal>
		struct IDImpl
		{
			MXT_STRING_CHECK(S);
			MXT_STRING_CHECK(R);

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);

				if(i1 == i2)
				{
					return false;
				}
				else if(Literal ? *i1 == R::toString()
								: MXT_REGEX_SCOPE::regex_match(*i1, MXT_REGEX_SCOPE::regex(R::toString())))
				{
					st[S::toString()] = *i1;

					++i1;

					return true;
				}
				else
				{
					return false;
				}
			}

			typedef IDImpl<SymTable, Out, R, S, Literal> type;
		};

// # ---------------------------------------------------------------------------

		template<typename SymTable, typename Out, template<typename, typename> class F>
		struct HookImpl
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);

				F<SymTable, Out>::run(st, o);

				return true;
			}

			typedef HookImpl<SymTable, Out, F> type;
		};

		struct End
		{
			template<typename ST, typename O>
			struct Make
			{
				typedef EndImpl<ST, O> type;
			};
		};

		struct Empty
		{
			template<typename ST, typename O>
			struct Make
			{
				typedef EmptyImpl<ST, O> type;
			};
		};

		struct IDBase { };

		template<typename R = ID_ALL_S, typename S = ID_DEF_S, bool L = false>
		struct ID : IDBase
		{
			template<typename ST, typename O>
			struct Make
			{
				typedef IDImpl<ST, O, R, S, L> type;
			};
		};

		template<typename R, typename S = LIT_DEF_S>
		using Literal = ID<R, S, true>;

		struct HookBase { };

		template<template<typename, typename> class F>
		struct Hook : HookBase
		{
			template<typename ST, typename O>
			struct Make
			{
				typedef HookImpl<ST, O, F> type;
			};
		};

		template<typename S>
		struct PrintVar : HookBase
		{
			MXT_STRING_CHECK(S);

			template<typename ST, typename O>
			struct DoPrint
			{
				static void run(ST& st, O& o)
				{
					o << st.at(S::toString());
				}
			};

			template<typename ST, typename O>
			struct Make
			{
				typedef HookImpl<ST, O, DoPrint> type;
			};
		};

		typedef PrintVar<ID_DEF_S> PrintID;

		template<typename S>
		struct Print : HookBase
		{
			MXT_STRING_CHECK(S);

			template<typename ST, typename O>
			struct DoPrint
			{
				static void run(ST& st, O& o)
				{
					o << S::toString();
				}
			};

			template<typename ST, typename O>
			struct Make
			{
				typedef HookImpl<ST, O, DoPrint> type;
			};
		};

// # ---------------------------------------------------------------------------

		template<int I, typename ST, typename O, typename R, typename L>
		struct Trans;

		template<typename ST, typename O, typename T, typename N>
		struct GetTrans
		{
			typedef Trans<N::value, ST, O, T, typename Get<T, N::value>::type> type;
		};

		template<int Idx, typename SymTable, typename Out, typename Reference, typename T, typename TT>
		struct Trans<Idx, SymTable, Out, Reference, TypeList<T, TT>>
		{
			enum { index = Idx };

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);

				typedef typename T::head H;
				typedef typename If<IsDerived<Num, H>, GetTrans<SymTable, Out, Reference, H>, H>::type head;
				typedef typename T::tail rest;

				if(head::matches(st, i1, i2, o))
				{
					MXT_LEX_LOG("@'%s': Entered translation #%d", MXT_LOGC, index);
					return Trans<Idx, SymTable, Out, Reference, TypeList<rest, FAIL>>::matches(st, i1, i2, o);
				}
				else
				{
					MXT_LEX_LOG("@'%s': Retry translation #%d", MXT_LOGC, index);
					return Trans<Idx, SymTable, Out, Reference, TT>::matches(st, i1, i2, o);
				}

				return false;
			}
		};

		template<int Idx, typename SymTable, typename Out, typename Reference>
		struct Trans<Idx, SymTable, Out, Reference, TypeList<NIL, FAIL>>
		{
			enum { index = Idx };

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);

				MXT_LEX_LOG("@'%s': Successfully completed translation #%d", MXT_LOGC, index);

				return true;
			}
		};

		template<int Idx, typename SymTable, typename Out, typename Reference>
		struct Trans<Idx, SymTable, Out, Reference, FAIL>
		{
			enum { index = Idx };

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);

				std::string s("ERR: Unexpected token '" + (i1 == i2 ? "#EOF#" : *i1) + "' in translation #" + lexical_cast<std::string>(index));

				MXT_LEX_LOG("%s", s.data());

				throw s;
			}
		};

		template<int Idx, typename SymTable, typename Out, typename Reference>
		struct Trans<Idx, SymTable, Out, Reference, NIL>
		{
			enum { index = Idx };

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2, Out& o)
			{
				MXT_DEREF_CHECK(I);

				MXT_LEX_LOG("@'%s': Translation #%d does not match!", MXT_LOGC, index);

				return false;
			}
		};

		template<typename RawTransList, typename Out = std::ostream, typename SymTable = std::map<std::string, std::string>>
		struct Parser
		{
			typedef MakeTypeList<End, Empty, IDBase, HookBase> CONV_REQ;

			template<typename T>
			struct Make
			{
				typedef typename T::template Make<SymTable, Out>::type type;
			};

			template<typename T>
			struct Convert
			{
				typedef typename If<IsIn<CONV_REQ, T, IsDerived>, Make<T>, Identity<T>>::type type;
			};

			typedef typename RecursiveTransform<RawTransList, Convert>::type TransList;

			template<typename T, int I = 0>
			struct MakeTrans;

			template<typename H, typename T, int I>
			struct MakeTrans<TypeList<H, T>, I>
			{
				typedef TypeList<Trans<I, SymTable, Out, TransList, H>, typename MakeTrans<T, I + 1>::type> type;
			};

			template<int I>
			struct MakeTrans<NIL, I>
			{
				typedef NIL type;
			};

			typedef typename MakeTrans<TransList>::type Reference;

//			template<typename T>
//			struct MakeTrans
//			{
//				typedef Trans<SymTable, Out, TransList, T> type;
//			};
//
//			typedef typename Transform<TransList, MakeTrans>::type Reference;

			template<typename I>
			static void parse(I&& i1, const I& i2, Out& o)
			{
				SymTable st;

				typedef typename Reference::head start;

				if(!start::matches(st, i1, i2, o))
				{
					throw std::string("ERR: Failed to match. Stopped at '" + (i1 == i2 ? "#END#" : *i1) + "'");
				}
			}
		};

#undef MXT_LOGC
#undef MXT_STRING_CHECK
#undef MXT_DEREF_CHECK
#undef MXT_LEX_LOG
	}
}

#endif

