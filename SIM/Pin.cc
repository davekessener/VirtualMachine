#include "Pin.h"

namespace sim
{
	Pin::Pin(void) : v_(0), w_(NULL)
	{
	}

	Pin::~Pin(void)
	{
	}

	V_t Pin::out(void) const
	{
		return v_;
	}

	void Pin::in(const V_t& v)
	{
		v_ = v;
	}

	void Pin::connect(Wire& w)
	{
		if(w_)
		{
			w_->disconnect(*this);
		}

		w_ = &w;
		w.connect(*this);
	}

	V_t Pin::passive(void) const
	{
		return w_ ? w_->out() : V_t(0);
	}
}

