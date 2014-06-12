#include "Chip.h"

namespace sim
{
	namespace
	{
		std::vector<V_t> captureState(const std::map<std::string, Wire::Wire_ptr>& w)
		{
			std::vector<V_t> r;
			r.reserve(w.size());

			for(auto p : w)
			{
				r.push_back(p.second->out());
			}

			return r;
		}

		bool checkState(const std::vector<V_t>& s1, const std::vector<V_t>& s2)
		{
			if(s1.size() != s2.size()) return false;

			for(int i = 0, s = s1.size() ; i < s ; ++i)
			{
				if(s1.at(i) != s2.at(i)) return false;
			}

			return true;
		}
	}

	Chip::Chip(void) : name_("_"), optimized_(false)
	{
	}

	Chip::~Chip(void)
	{
	}

	void Chip::tick(void)
	{
		if(optimized_)
		{
			std::vector<V_t> s;

			for(Chip_ptr &ch : chips_)
			{
				ch->enableOptimization();
			}

			do
			{
				s = captureState(wires_);

				tickImpl();
			} while(!checkState(s, captureState(wires_)));
		}
		else
		{
			tickImpl();
		}
	}
	
	void Chip::tickImpl(void)
	{
		for(Chip_ptr &ch : chips_)
		{
			ch->tick();
		}

		for(auto p : wires_)
		{
			p.second->update();
		}
	}

	void Chip::addChip(Chip_ptr c)
	{
		chips_.push_back(c);
	}

	Wire::Wire_ptr Chip::getNode(const std::string &idx)
	{
		if(!static_cast<bool>(wires_[idx]))
		{
			wires_[idx].reset(new Wire);
		}

		return wires_[idx];
	}

	void Chip::setInput(const std::string &idx)
	{
		Wire_ptr w = getNode(idx);
		Connector_ptr o(new Connector);

		w->connect(*o);

		input_.push_back(o);
	}

	void Chip::setOutput(const std::string &idx)
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

	void Chip::enableOptimization(void)
	{
		optimized_ = true;
	}

	bool Chip::isOptimized(void) const
	{
		return optimized_;
	}

	void Chip::setName(const std::string& n)
	{
		name_ = n;
	}

	const std::string& Chip::getName(void) const
	{
		return name_;
	}
}

