#ifndef SIM_CONDUCTABLE_H
#define SIM_CONDUCTABLE_H

#include "Sim.h"

namespace sim
{
	class Conductable
	{
		public:
			virtual V_t out( ) const = 0;
	};
}

#endif

