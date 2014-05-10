#include "Connector.h"

namespace sim
{
	Connector::Connector(void) : v_(0)
	{
	}

	void Connector::setInput(Wire::Wire_ptr w)
	{
		node_ = w;
	}

	V_t Connector::out(void) const
	{
		return static_cast<bool>(node_) ? node_->out() : v_;
	}

	void Connector::in(const V_t& v)
	{
		v_ = v;
	}
}

