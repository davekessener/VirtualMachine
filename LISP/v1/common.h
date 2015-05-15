#ifndef LISP_COMMON_H
#define LISP_COMMON_H

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr

namespace lisp
{
	typedef decltype(NULL) NULL_t;
	typedef NULL_t NIL;
}

#endif

