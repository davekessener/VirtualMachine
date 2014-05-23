#ifndef LISP_BIGNUM_H
#define LISP_BIGNUM_H

#include "common.h"
#include "auto_ptr.hpp"

namespace lisp
{
	class BigNum
	{
		public:
			BigNum( );
			BigNum(const BigNum&);
			~BigNum( );
		private:
			struct BNImpl;
			const auto_ptr<BNImpl> impl_;
	};
}

#endif

