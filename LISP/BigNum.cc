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

	BigNum::BigNum(void)
	{
	}

	BigNum::~BigNum(void)
	{
	}

// # ===========================================================================

	BigNum::BNImpl::BNImpl(void)
	{
	}

	BigNum::BNImpl::BNImpl(const BNImpl& i)
	{
	}

	BigNum::BNImpl::~BNImpl(void)
	{
	}
}

