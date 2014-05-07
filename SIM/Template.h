#ifndef SIM_TEMPLATE_H
#define IM_TEMPLATE_H

#include <sstream>
#include <vector>
#include "Sim.h"
#include "Chip.h"

namespace sim
{
	class Template
	{
		public:
		typedef std::shared_ptr<Template> Template_ptr;

		public:
			Template(const std::vector<std::string>&);
			~Template( );
			Chip::Chip_ptr instantiate( ) const;
		private:
			std::string name_;
			struct Scanner;
			std::vector<Scanner *> components_;
			std::vector<int> ins_, outs_;
	};
}

#endif

