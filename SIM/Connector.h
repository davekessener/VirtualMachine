#ifndef SIM_CONNECTOR_H
#define SIM_CONNECTOR_H

#include <memory>
#include "Sim.h"
#include "Conductable.h"
#include "Wire.h"

namespace sim
{
	class Connector : public Conductable
	{
		public:
		typedef std::shared_ptr<Connector> Connector_ptr;

		public:
			Connector( );
			void setInput(Wire::Wire_ptr);
			V_t out( ) const;
			void in(const V_t&);
		private:
			Wire::Wire_ptr node_;
			V_t v_;
	};
}

#endif

