#ifndef SIM_CHIP_H
#define SIM_CHIP_H

#include <string>
#include <memory>
#include "Sim.h"
#include "Wire.h"
#include "Connector.h"

namespace sim
{
	class Chip
	{
		public:
		typedef std::shared_ptr<Chip> Chip_ptr;
		typedef Wire::Wire_ptr Wire_ptr;
		typedef Connector::Connector_ptr Connector_ptr;
		typedef std::shared_ptr<ConnectorInput> CIn_ptr;
		typedef std::shared_ptr<ConnectorOutput> COut_ptr;

		public:
			Chip( );
			virtual ~Chip( );
			virtual void tick( );
			void addChip(Chip_ptr);
			Wire_ptr getNode(int);
			void setInput(int);
			void setOutput(int);
		protected:
			std::vector<Chip_ptr> chips_;
			std::vector<COut_ptr> input_;
			std::vector<CIn_ptr>  output_;
			std::vector<Wire_ptr> wires_;
	};
}

#endif

