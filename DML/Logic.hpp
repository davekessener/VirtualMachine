#ifndef DML_LOGIC_H
#define DML_LOGIC_H

#include "DML.h"
#include "Integral.hpp"

namespace dml
{
	namespace
	{
		template<bool C, typename T1, typename T2>
		struct _if;

		template<typename T1, typename T2>
		struct _if<true, T1, T2>
		{
			typedef T1 RET;
		};

		template<typename T1, typename T2>
		struct _if<false, T1, T2>
		{
			typedef T2 RET;
		};

		template<bool C, typename T1, typename T2>
		struct _if_d;

		template<typename T1, typename T2>
		struct _if_d<true, T1, T2>
		{
			typedef typename T1::RET RET;
		};

		template<typename T1, typename T2>
		struct _if_d<false, T1, T2>
		{
			typedef typename T2::RET RET;
		};
	}

	template<typename C, typename T1, typename T2>
	struct If
	{
		typedef typename _if<C::VAL, T1, T2>::RET RET;
	};

	template<typename C, typename T1, typename T2>
	struct If_
	{
		typedef typename _if_d<C::VAL, T1, T2>::RET RET;
	};

	template<typename T, typename ... TT>
	struct And
	{
		struct RET { enum { VAL = T::VAL && And<TT...>::VAL }; };
		enum { VAL = RET::VAL };
	};

	template<typename T>
	struct And<T>
	{
		struct RET { enum { VAL = T::VAL }; };
		enum { VAL = RET::VAL };
	};

	template<typename T, typename ... TT>
	struct Or
	{
		struct RET { enum { VAL = T::VAL || Or<TT...>::VAL }; };
		enum { VAL = RET::VAL };
	};

	template<typename T>
	struct Or<T>
	{
		struct RET { enum { VAL = T::VAL }; };
		enum { VAL = RET::VAL };
	};
}

#endif

