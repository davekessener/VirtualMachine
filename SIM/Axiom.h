#ifndef SIM_AXIOM_H
#define SIM_AXIOM_H

#include "Sim.h"
#include "Chip.h"

namespace sim
{
	class ChipNot : public Chip
	{
		public:
			void tick( );
	};

	class ChipAnd : public Chip
	{
		public:
			void tick( );
	};

	class ChipOr : public Chip
	{
		public:
			void tick( );
	};

	class ChipXOr : public Chip
	{
		public:
			void tick( );
	};
}

#endif

