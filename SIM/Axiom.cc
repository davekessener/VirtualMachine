#include "Axiom.h"

namespace sim
{
	void ChipNot::tick(void)
	{
		output_.at(0)->in(!input_.at(0)->out());

		Chip::tick();
	}

	void ChipAnd::tick(void)
	{
		assert(!input_.empty());
		V_t v(1);

		for(std::shared_ptr<ConnectorOutput>& p : input_)
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

		for(std::shared_ptr<ConnectorOutput>& p : input_)
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
}

