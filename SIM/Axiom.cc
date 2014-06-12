#include "Axiom.h"

namespace sim
{
	Connector::Connector_ptr OneOutChip::getOutput(int idx)
	{
		assert(!idx);

		if(output_.empty()) output_.push_back(Connector_ptr(new Connector));

		return output_.at(idx);
	}

	Connector::Connector_ptr ManyInChip::getInput(int idx)
	{
		while(idx >= input_.size())
		{
			input_.push_back(Connector_ptr(new Connector));
		}

		return input_.at(idx);
	}

	Connector::Connector_ptr ChipHI::getOutput(int idx)
	{
		Connector::Connector_ptr o = OneOutChip::getOutput(idx);

		o->in(V_t(1));

		return o;
	}

	void ChipNot::tick(void)
	{
		output_.at(0)->in(!input_.at(0)->out());

		Chip::tick();
	}

	void ChipAnd::tick(void)
	{
		assert(!input_.empty());
		V_t v(1);

		for(Connector::Connector_ptr& p : input_)
		{
			if(!p->out())
			{
				v = V_t(0);
				break;
			}
		}

		output_.at(0)->in(v);

		Chip::tick();
	}

	void ChipOr::tick(void)
	{
		assert(!input_.empty());
		V_t v(0);

		for(Connector::Connector_ptr& p : input_)
		{
			if(p->out())
			{
				v = V_t(1);
				break;
			}
		}

		output_.at(0)->in(v);

		Chip::tick();
	}

	void ChipXOr::tick(void)
	{
		assert(input_.size()==2);

		output_.at(0)->in(input_.at(0)->out() ^ input_.at(1)->out());

		Chip::tick();
	}

	void ChipNAnd::tick(void)
	{
		assert(!input_.empty());
		V_t v(0);

		for(Connector::Connector_ptr& p : input_)
		{
			if(!p->out())
			{
				v = V_t(1);
				break;
			}
		}

		output_.at(0)->in(v);

		Chip::tick();
	}
}

