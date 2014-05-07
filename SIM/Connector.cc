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

// # ===========================================================================
//
//	Connector::Connector(Wire::Wire_ptr w) : node_(w)
//	{
//	}
//
//	void Connector::connect(Connector_ptr p)
//	{
//		pin_ = p;
//	}
//
//	ConnectorInput::ConnectorInput(Wire::Wire_ptr w) : Connector(w), v_(0)
//	{
//	}
//
//	V_t ConnectorInput::out(void) const
//	{
//		return v_;
//	}
//
//	void ConnectorInput::in(const V_t& v)
//	{
//		v_ = v;
//	}
//
//	ConnectorOutput::ConnectorOutput(Wire::Wire_ptr w) : Connector(w)
//	{
//	}
//
//	V_t ConnectorOutput::out(void) const
//	{
//		assert(static_cast<bool>(pin_));
//
//		return pin_->node_->out();
//	}
}

