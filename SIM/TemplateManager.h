#ifndef SIM_TEMPLATEMANAGER_H
#define SIM_TEMPLATEMANAGER_H

#include <map>
#include <fstream>
#include "Template.h"
#include "AxiomTemplate.hpp"
#include "Axiom.h"

namespace sim
{
	class TemplateManager
	{
		public:
			static TemplateManager& instance( );
			static Template::Template_ptr getTemplate(const std::string&);
		private:
			TemplateManager( );
			~TemplateManager( );
			TemplateManager(const TemplateManager&);
			TemplateManager& operator=(const TemplateManager&);

			static std::vector<std::string> readFile(const std::string&);
			Template::Template_ptr getTemplate_(const std::string&);

			std::map<std::string, Template::Template_ptr> templates_;

			static const std::string TEMPLATE_EXT;
	};
}

#endif

