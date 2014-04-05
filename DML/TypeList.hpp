#ifndef DML_TYPELIST_H
#define DML_TYPELIST_H

#include "DML.h"
#include "Types.hpp"
#include "Traits.hpp"

namespace dml
{
	template<typename H, typename T = NULL_t>
	struct TypeList
	{
		typedef H HEAD;
		typedef T TAIL;
	};

	namespace typelist
	{
		template<template<typename, typename> class, typename ...>
		struct Make;

		template<template<typename, typename> class P, typename T, typename ... TT>
		struct Make<P, T, TT...>
		{
			typedef P<T, typename Make<P, TT...>::RET> RET;
		};

		template<template<typename, typename> class P>
		struct Make<P>
		{
			typedef NULL_t RET;
		};

		template<typename ... T>
		struct MakeTypeList
		{
			typedef typename Make<TypeList, T...>::RET RET;
		};

// # ---------------------------------------------------------------------------
	
		template<typename T, typename TT, int i = 0>
		struct Find
		{
			typedef typename Find<typename traits::Strip<T>::RET, TT, i>::RET RET;
			enum { VAL = RET::VAL };
		};

		template<typename H, typename T, typename TT, int i>
		struct Find<TypeList<H, T>, TT, i>
		{
			struct RET { enum { VAL = types::Equal<H, TT>::RET::VAL ? i : Find<T, TT, i + 1>::RET::VAL }; };
			enum { VAL = RET::VAL };
		};

		template<typename TT, int i>
		struct Find<NULL_t, TT, i>
		{
			struct RET { enum { VAL = -1 }; };
			enum { VAL = RET::VAL };
		};

// # ---------------------------------------------------------------------------

		template<typename, template<typename, typename> class>
		struct Transfer;

		template<typename H, typename T, template<typename, typename> class P>
		struct Transfer<TypeList<H, T>, P>
		{
			typedef P<H, typename Transfer<T, P>::RET> RET;
		};

		template<typename H, template<typename, typename> class P>
		struct Transfer<TypeList<H, NULL_t>, P>
		{
			typedef H RET;
		};
	}
}

#endif

