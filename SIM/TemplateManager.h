#ifndef SIM_TEMPLATEMANAGER_H
#define SIM_TEMPLATEMANAGER_H

#include <map>
#include <fstream>
#include "Template.h"

namespace sim
{
	class TemplateManager
	{
		public:
			static TemplateManager& instance( );
			Template::Template_ptr getTemplate(const std::string&);
		private:
			TemplateManager( );
			~TemplateManager( );
			TemplateManager(const TemplateManager&);
			TemplateManager& operator=(const TemplateManager&);

			std::vector<std::string> readFile(const std::string&);

			std::map<std::string, Template::Template_ptr> templates_;
	};
}

#endif

