#ifndef SIM_TEMPLATE_H
#define SIM_TEMPLATE_H

#include <sstream>
#include <vector>
#include <string>
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
			virtual ~Template( );
			virtual Chip::Chip_ptr instantiate( ) const;
		private:
			std::string name_;
			struct Scanner;
			std::vector<Scanner *> components_;
			std::vector<std::string> ins_, outs_, pre_;
			bool isOptimized_;
	};
}

#endif

