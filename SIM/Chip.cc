#include "Chip.h"

namespace sim
{
	Chip::Chip(void)
	{
	}

	Chip::~Chip(void)
	{
	}

	void Chip::tick(void)
	{
		for(Chip_ptr &ch : chips_)
		{
			ch->tick();
		}

		for(Wire_ptr &w : wires_)
		{
			w->update();
		}
	}

	void Chip::addChip(Chip_ptr c)
	{
		chips_.push_back(c);
	}

	Wire::Wire_ptr Chip::getNode(int idx)
	{
		while(idx >= wires_.size())
		{
			wires_.push_back(Wire_ptr(new Wire()));
		}

		return wires_.at(idx);
	}

	void Chip::setInput(int idx)
	{
		Wire_ptr w = getNode(idx);
		COut_ptr o(new ConnectorOutput(w));

//		w->connect(*o);

		input_.push_back(o);
	}

	void Chip::setOutput(int idx)
	{
		Wire_ptr w = getNode(idx);
		CIn_ptr i(new ConnectorInput(w));

		w->connect(*i);

		output_.push_back(i);
	}
}

