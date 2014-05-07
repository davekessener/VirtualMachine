#ifndef SIM_AXIOM_H
#define SIM_AXIOM_H

#include "Sim.h"
#include "Chip.h"

namespace sim
{
	class OneOutChip : public virtual Chip
	{
		public:
		using Chip::Connector_ptr;

		public:
			virtual Connector_ptr getOutput(int);
	};

	class ManyInChip : public virtual Chip
	{
		public:
		using Chip::Connector_ptr;

		public:
			virtual Connector_ptr getInput(int);
	};

	class ChipHI : public OneOutChip
	{
		public:
			void tick( );
	};

	class ChipNot : public OneOutChip
	{
		public:
			void tick( );
	};

	class ChipAnd : public OneOutChip, public ManyInChip
	{
		public:
			void tick( );
	};

	class ChipOr : public OneOutChip, public ManyInChip
	{
		public:
			void tick( );
	};

	class ChipXOr : public OneOutChip, public ManyInChip
	{
		using Chip::Connector_ptr;

		public:
			void tick( );
			Connector_ptr getInput(int);
	};
}

#endif

