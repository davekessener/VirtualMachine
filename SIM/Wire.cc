#include "Wire.h"

namespace sim
{
	Wire::Wire(void) : pins_(), v_(0)
	{
	}

	Wire::~Wire(void)
	{
	}

	void Wire::connect(const Conductable& c)
	{
		assert(find(c)==pins_.end());
		pins_.push_back(&c);
	}

	void Wire::disconnect(const Conductable& c)
	{
		auto i = find(c);

		assert(i!=pins_.end());

		pins_.erase(i);
	}

	V_t Wire::out(void) const
	{
		return v_;
	}

	void Wire::update(void)
	{
		v_ = V_t(0);

		for(const Conductable *c : pins_)
		{
			v_ += c->out();
		}
	}

	Wire::cond_vec_t::iterator Wire::find(const Conductable& c)
	{
		auto end = pins_.end();
		for(auto i = pins_.begin() ; i != end ; ++i)
		{
			if(*i == &c) return i;
		}

		return end;
	}
}

