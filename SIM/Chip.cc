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
		Connector_ptr o(new Connector);

		w->connect(*o);

		input_.push_back(o);
	}

	void Chip::setOutput(int idx)
	{
		Wire_ptr w = getNode(idx);
		Connector_ptr i(new Connector);

		i->setInput(w);

		output_.push_back(i);
	}

	Connector::Connector_ptr Chip::getInput(int idx)
	{
		return input_.at(idx);
	}

	Connector::Connector_ptr Chip::getOutput(int idx)
	{
		return output_.at(idx);
	}

	int Chip::inpinCount(void) const
	{
		return input_.size();
	}

	int Chip::outpinCount(void) const
	{
		return output_.size();
	}
}

