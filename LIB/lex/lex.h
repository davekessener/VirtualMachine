#ifndef DAV_LEX_H
#define DAV_LEX_H

#include <type_traits>
#include <string>

//namespace dav
//{
//	namespace lex
//	{
		template<typename T>
		struct Identity
		{
			typedef T type;
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

			typedef EmptyImpl<SymTable> type;
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

			typedef LiteralImpl<SymTable, S> type;
		};

		struct LiteralBase { };

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

			typedef IDImpl<SymTable, S> type;
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

			typedef HookImpl<SymTable, F> type;
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
		struct Print : HookBase
		{
			MXT_STRING_CHECK(S);

			template<typename ST>
			struct DoPrint
			{
				static void run(ST& st)
				{
					st << st.at(S::toString());
				}
			};

			template<typename ST>
			struct Make
			{
				typedef HookImpl<ST, DoPrint> type;
			};
		};

		typedef Print<ID_DEF_S> PrintID;

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
				typedef typename If<IsDerived<Num, H>, GetTrans<SymTable, Reference, H>, H>::type head;
				typedef typename T::tail rest;

				if(head::matches(st, i1, i2))
				{
					return Trans<SymTable, Reference, TypeList<rest, FAIL>>::matches(st, i1, i2);
				}
				else
				{
					return Trans<SymTable, Reference, TT>::matches(st, i1, i2);
				}

				return false;
			}
		};

		template<typename SymTable, typename Reference>
		struct Trans<SymTable, Reference, TypeList<NIL, FAIL>>
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				return true;
			}
		};

		template<typename SymTable, typename Reference>
		struct Trans<SymTable, Reference, FAIL>
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
			{
				MXT_DEREF_CHECK(I);

				throw std::string("ERR: Unexpected eos @'" + *i1 + "'");
			}
		};

		template<typename SymTable, typename Reference>
		struct Trans<SymTable, Reference, NIL>
		{
			template<typename I>
			static bool matches(SymTable& st, I& i1, const I& i2)
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
			struct Make
			{
				typedef typename T::template Make<SymTable>::type type;
			};

			template<typename T>
			struct Convert
			{
				typedef typename If<IsIn<CONV_REQ, T, IsDerived>, Make<T>, Identity<T>>::type type;
			};

			typedef typename RecursiveTransform<RawTransList, Convert>::type TransList;

			template<typename T>
			struct MakeTrans
			{
				typedef Trans<SymTable, TransList, T> type;
			};

			typedef typename Transform<TransList, MakeTrans>::type Reference;

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
//	}
//}

#endif

