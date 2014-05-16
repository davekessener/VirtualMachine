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

		public:
			Chip( );
			virtual ~Chip( );
			virtual void tick( );
			void addChip(Chip_ptr);
			Wire_ptr getNode(int);
			void setInput(int);
			void setOutput(int);
			virtual Connector_ptr getInput(int);
			virtual Connector_ptr getOutput(int);
			int inpinCount( ) const;
			int outpinCount( ) const;
			void enableOptimization( );
			void setName(const std::string&);
		protected:
			std::vector<Chip_ptr> chips_;
			std::vector<Connector_ptr> input_, output_;
			std::vector<Wire_ptr> wires_;
			std::string name_;
			bool optimized_;

			void tickImpl( );
	};
}

#endif

