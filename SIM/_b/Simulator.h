#ifndef SIM_SIMULATOR_H
#define SIM_SIMULATOR_H

#include <iostream>
#include "Sim.h"
#include "Chip.h"
#include "Template.h"
#include "TemplateManager.h"

namespace sim
{
	class Simulator
	{
		public:
			Simulator(const std::string&);
			~Simulator( );
			void run( );
		private:
			Template::Template_ptr template_;
	};
}

#endif

