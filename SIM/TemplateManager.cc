#include "TemplateManager.h"

const std::string sim::TemplateManager::TEMPLATE_EXT(".chip");

namespace sim
{
	TemplateManager& TemplateManager::instance(void)
	{
		static TemplateManager i;

		return i;
	}

	TemplateManager::TemplateManager(void)
	{
		templates_["HI"].reset(new AxiomTemplate<ChipHI>);
		templates_["NOT"].reset(new AxiomTemplate<ChipNot>);
		templates_["AND"].reset(new AxiomTemplate<ChipAnd>);
		templates_["OR"].reset(new AxiomTemplate<ChipOr>);
		templates_["XOR"].reset(new AxiomTemplate<ChipXOr>);
	}

	TemplateManager::~TemplateManager(void)
	{
	}

	Template::Template_ptr TemplateManager::getTemplate(const std::string& name)
	{
		return instance().getTemplate_(name);
	}

	Template::Template_ptr TemplateManager::getTemplate_(const std::string& name)
	{
		if(!static_cast<bool>(templates_[name]))
		{
			templates_[name].reset(new Template(readFile(name)));
		}

		return templates_[name];
	}

	std::vector<std::string> TemplateManager::readFile(const std::string& fn)
	{
		std::ifstream in(fn + TEMPLATE_EXT);
		std::vector<std::string> c;

		assert(in.is_open());

		c.push_back(fn);

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

