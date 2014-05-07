#include "TemplateManager.h"

namespace sim
{
	TemplateManager& TemplateManager::instance(void)
	{
		static TemplateManager i;

		return i;
	}

	TemplateManager::TemplateManager(void)
	{
	}

	TemplateManager::~TemplateManager(void)
	{
	}

	Template::Template_ptr TemplateManager::getTemplate(const std::string& name)
	{
		if(!static_cast<bool>(templates_[name]))
		{
			templates_[name].reset(new Template(readFile(name)));
		}

		return templates_[name];
	}

	std::vector<std::string> TemplateManager::readFile(const std::string& fn)
	{
		std::ifstream in(fn);
		std::vector<std::string> c;

		assert(in.is_open());

		while(in)
		{
			std::string s;
			getline(in, s);
			if(!s.empty()) c.push_back(s);
		}

		in.close();

		return c;
	}
}

