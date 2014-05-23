#include "BigNum.h"

namespace lisp
{
	struct BigNum::BNImpl
	{
		public:
			BNImpl( );
			BNImpl(const BNImpl&);
			~BNImpl( );
		private:
	};

	BigNum::BigNum(void) : impl_(new BNImpl)
	{
	}

	BigNum::BigNum(const BigNum& n) : impl_(new BNImpl(*n.impl_))
	{
	}

	BigNum::~BigNum(void)
	{
	}
}

