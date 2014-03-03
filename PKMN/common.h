#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
#define NULL_t decltype(nullptr)

namespace Controls
{
	enum
	{
		NONE,
		A,
		B,
		X,
		Y,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		START,
		SELECT,
		L,
		R
	};
}

enum class Direction : std::uint32_t
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

template<typename T>
struct type_traits;

template<typename T>
struct type_traits<const T>
{
	typedef typename type_traits<T>::base_type base_type;
};

template<typename T>
struct type_traits<T *>
{
	typedef typename type_traits<T>::base_type base_type;
};

template<typename T>
struct type_traits<T&>
{
	typedef typename type_traits<T>::base_type base_type;
};

template<typename T>
struct type_traits
{
	typedef T base_type;
};

#endif

