#include "Manager.h"
#include <vector>
#include <string>
#include "../File.h"
#include <aux>
#include "StringList.h"
#include "Viewer.h"

namespace editor { namespace surface {

namespace
{
	StringList& toSL(Surface_ptr p) { return *dynamic_cast<StringList *>(&*p); }
	Viewer& toV(Surface_ptr p) { return *dynamic_cast<Viewer *>(&*p); }
}

void Manager::i_doInit(void)
{
	list_.reset(new StringList([this](const std::string& s) { updateContent(s); }));
	list_->init(0, 0, width() / 6, height());
	addChild(list_);

	content_.reset(new Viewer);
	content_->init(width() / 6, 0, width() - width() / 6, height());
	addChild(content_);
}

void Manager::i_doUpdate(int d)
{
}

void Manager::i_doPrerender(void)
{
	using dav::aux::lexical_cast;

	std::vector<std::string> v;

	for(auto i = File::begin(), e = File::end() ; i != e ; ++i)
	{
		v.push_back(lexical_cast<std::string>(i->ID()) + ": " + i->name());
	}

	toSL(list_).load(v.cbegin(), v.cend());
}

void Manager::i_doRender(void) const
{
}

void Manager::updateContent(const std::string& map)
{
}

}}

