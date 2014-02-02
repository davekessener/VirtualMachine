#ifndef TYPELIST_HPP
#define TYTELIST_HPP

#include <iostream>
#include "Loki.h"

template<class H, class T>
struct TypeList
{
	typedef H head;
	typedef T tail;
};

namespace TL
{
	template<class ... T>
	struct Construct;

	template<class H, class ... T>
	struct Construct<H, T...>
	{
		typedef TypeList<H, typename Construct<T...>::list> list;
	};

	template<>
	struct Construct<>
	{
		typedef NULL_t list;
	};

// # ---------------------------------------------------------------------------
	
	template<class ... TL>
	struct Union;

	template<class H, class T>
	struct Union<TypeList<H, T>>
	{
		typedef TypeList<H, T> list;
	};

	template<class H, class T, class ... TL>
	struct Union<TypeList<H, T>, TL...>
	{
		typedef TypeList<H, typename Union<T, TL...>::list> list;
	};

	template<class H, class T, class ... TL>
	struct Union<NULL_t, TypeList<H, T>, TL...>
	{
		typedef typename Union<TypeList<H, T>, TL...>::list list;
	};

// # ---------------------------------------------------------------------------

	template<class TL>
	struct Length;

	template<class H, class T>
	struct Length<TypeList<H, T>>
	{
		enum { length = 1 + Length<typename TypeList<H, T>::tail>::length };
	};

	template<>
	struct Length<NULL_t>
	{
		enum { length = 0 };
	};
}

#endif

