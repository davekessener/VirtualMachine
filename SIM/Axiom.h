#ifndef SIM_AXIOM_H
#define SIM_AXIOM_H

#include "Sim.h"
#include "Chip.h"

namespace sim
{
	class OptimizedChip : public Chip
	{
		public:
			OptimizedChip( ) { enableOptimization(); }
	};

	class OneOutChip : public virtual OptimizedChip
	{
		public:
		using Chip::Connector_ptr;

		public:
			virtual Connector_ptr getOutput(int);
	};

	class ManyInChip : public virtual OptimizedChip
	{
		public:
		using Chip::Connector_ptr;

		public:
			virtual Connector_ptr getInput(int);
	};

	template<size_t V>
	class SomeInChip : public ManyInChip
	{
		public:
		using Chip::Connector_ptr;

		public:
			virtual Connector_ptr getInput(int);
	};

	template<size_t V>
	Connector::Connector_ptr SomeInChip<V>::getInput(int idx)
	{
		assert(idx>=0&&idx<V);
		return ManyInChip::getInput(idx);
	}

	class ChipHI : public OneOutChip
	{
		using Chip::Connector_ptr;

		public:
			ChipHI( ) { setName("HI"); }
			virtual Connector_ptr getOutput(int);
	};

	class ChipNot : public OneOutChip, public SomeInChip<1>
	{
		public:
			ChipNot( ) { setName("NOT"); }
			void tick( );
	};

	class ChipAnd : public OneOutChip, public ManyInChip
	{
		public:
			ChipAnd( ) { setName("AND"); }
			void tick( );
	};

	class ChipOr : public OneOutChip, public ManyInChip
	{
		public:
			ChipOr( ) { setName("OR"); }
			void tick( );
	};

	class ChipXOr : public OneOutChip, public SomeInChip<2>
	{
		public:
			ChipXOr( ) { setName("XOR"); }
			void tick( );
	};

	class ChipNAnd : public OneOutChip, public ManyInChip
	{
		public:
			ChipNAnd( ) { setName("NAND"); }
			void tick( );

	};
}

#endif

