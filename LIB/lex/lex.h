#ifndef DAV_LEX_H
#define DAV_LEX_H

#include <type_traits>
#include <string>

namespace dav
{
	namespace lex
	{
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

// # ===========================================================================

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
			typedef typename Get<typename T::tail, I - 1>::type type;
		};

		template<typename T>
		struct Get<T, 0>
		{
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

// # ===========================================================================

#define MXT_DEREF_CHECK(I) static_assert(DereferencesToString<I>::value, "ERR: Parser operates on std::strings only!")
#define MXT_STRING_CHECK(S) static_assert(IsDerived<StringBase, S>::value, "ERR: S needs to be derived from StringBase!")

		typedef String<'*'> ID_DEF_S;

		struct FAIL { };

		template<typename SymTable>
		struct EmptyImpl
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);
				return true;
			}
		};

		struct Empty
		{
			template<typename ST>
			struct Make
			{
				typedef EmptyImpl<ST> type;
			};
		};

		template<typename SymTable, typename S>
		struct LiteralImpl
		{
			MXT_STRING_CHECK(S);

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				if(i1 == i2)
				{
					return false;
				}
				else if(*i1 == S::toString())
				{
					++i1;
					return true;
				}
				else
				{
					return false;
				}
			}
		};

		struct Literalbase { };

		template<typename S>
		struct Literal : LiteralBase
		{
			template<typename ST>
			struct Make
			{
				typedef LiteralImpl<ST, S> type;
			};
		};

		template<typename SymTable, typename S>
		struct IDImpl
		{
			MXT_STRING_CHECK(S);

			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				if(i1 == i2)
				{
					return false;
				}
				else
				{
					st[S::toString()] = *i1;

					++i1;

					return true;
				}
			}
		};

		struct IDBase { };

		template<typename S = ID_DEF_S>
		struct ID : IDBase
		{
			template<typename ST>
			struct Make
			{
				typedef IDImpl<ST, S> type;
			};
		};

		template<typename SymTable, template<typename> class F>
		struct HookImpl
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				F<SymTable>::run(st);

				return true;
			}
		};

		struct HookBase { };

		template<template<typename> class F>
		struct Hook : HookBase
		{
			template<typename ST>
			struct Make
			{
				typedef HookImpl<ST, F> type;
			};
		};

		template<typename S>
		struct PrintHook : HookBase
		{
			MXT_STRING_CHECK(S);

			template<typename ST>
			struct Print
			{
				static void run(ST& st)
				{
					st << st.at(S::toString());
				}
			};

			template<typename ST>
			struct Make
			{
				typedef HookImpl<ST, Print> type;
			};
		};

		typedef PrintHook<ID_DEF_S> PrintID;

// # ---------------------------------------------------------------------------

		template<typename ST, typename R, typename L>
		struct Trans;

		template<typename ST, typename T, typename N>
		struct GetTrans
		{
			typedef Trans<ST, T, typename Get<T, N::value>::type> type;
		};

		template<typename SymTable, typename Reference, typename T, typename TT>
		struct Trans<SymTable, Reference, TypeList<T, TT>>
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				typedef typename T::head H;
				typedef typename If<IsDerived<H, Num>, GetTrans<SymTable, Reference, H>, H>::type head;
				typedef typename T::tail rest;

				if(head::matches(st, i1, i2))
				{
					return Trans<SymTable, Reference, TypeList<rest, FAIL>>::matches(st, i1, i2);
				}
				else
				{
					return Trans<SymTable, Reference, TT>::matches(st, i1, i2);
				}
			}
		};

		template<typename SymTable, typename Reference>
		struct Trans<SymTable, Reference, TypeList<NIL, FAIL>>
		{
			template<typename I>
			static void matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				return true;
			}
		};

		template<typename SymTable, typename Reference>
		struct Trans<SymTable, Reference, FAIL>
		{
			template<typename I>
			static void matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				throw std::string("ERR: Unexpected eos @'" + *i1 + "'");
			}
		};

		template<typename SymTable, typename Reference>
		struct Trans<SymTable, Reference, NIL>
		{
			template<typename I>
			static void matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				return false;
			}
		};

		template<typename SymTable, typename RawTransList>
		struct Parser
		{
			typedef MakeTypeList<Empty, LiteralBase, IDBase, HookBase> CONV_REQ;

			template<typename T>
			struct MakeTrans
			{
				typedef Trans<SymTable, RawTransList, T> type;
			};

			typedef typename Transform<RawTransList, MakeTrans>::type TransList;

			template<typename T>
			struct Convert
			{
				typedef typename If<IsIn<CONV_REQ, T, IsDerived>, T::template Make<SymTable>>::type;
			};

			template<typename T, typename TT>
			struct Convert<TypeList<T, TT>>
			{
				typedef typename Transform<TypeList<T, TT>, Convert>::type type;
			};

			typedef typename Transform<TransList, Convert>::type Reference;

			template<typename I>
			static void parse(I&& i1, const I& i2)
			{
				SymTable st;

				typedef typename Reference::head start;

				if(!start::matches(st, i1, i2))
				{
					throw std::string("ERR: Failed to match. Stopped at '" + (i1 == i2 ? "#END#" : *i1) + "'");
				}
			}
		};

#undef MXT_STRING_CHECK
#undef MXT_DEREF_CHECK
	}
}

#endif

