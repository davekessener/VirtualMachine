#include "Simulator.h"

namespace sim
{
	Simulator::Simulator(const std::string& name) : template_(TemplateManager::getTemplate(name))
	{
	}

	Simulator::~Simulator(void)
	{
	}

	namespace
	{
		void log(Chip::Chip_ptr ch)
		{
			for(int i = 0 ; i < ch->outpinCount() ; ++i)
			{
//				std::cout << "Pin #" << i << ": " << (static_cast<bool>(ch->getOutput(i)->out()) ? "HI" : "LO") << std::endl;
				std::cout << "Pin #" << i << ": " << std::flush;
				Connector::Connector_ptr c = ch->getOutput(i);
				V_t v = c->out();
				if(static_cast<bool>(v))
				{
					std::cout << "HI";
				}
				else
				{
					std::cout << "LO";
				}

				std::cout << std::endl;
			}
		}
	}

	void Simulator::run(void)
	{
		Chip::Chip_ptr ch(template_->instantiate());

		for(int i = 0 ; i < 10 ; ++i)
		{
			std::cout << "Tick #" << i << std::endl;
			log(ch);
			std::cout << std::endl;
			ch->tick();
		}
	}
}

