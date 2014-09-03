#include "Manager.h"
#include <vector>
#include <string>
#include "../File.h"
#include "../Controller.h"
#include <aux>
#include "StringList.h"
#include "Viewer.h"
#include "Tileset.h"
#include "Scrollable.h"

namespace editor { namespace surface {

namespace
{
	inline StringList& toSL(Surface_ptr p) { return *dynamic_cast<StringList *>(&*p); }
	inline Viewer& toV(Surface_ptr p) { return *dynamic_cast<Viewer *>(&*p); }
	inline Tileset& toTS(Surface_ptr p) { return *dynamic_cast<Tileset *>(&*p); }
}

void Manager::i_doInit(void)
{
	list_.reset(new StringList([this](const std::string& s) { updateContent(s); }));
	list_->init(0, 0, width() / 8, height());
	addChild(list_);

	Surface_ptr p(new Scrolling);
	tileset_.reset(new Tileset);
	std::dynamic_pointer_cast<Scrolling>(p)->setScrollable(std::dynamic_pointer_cast<Scrollable>(tileset_));
	p->init(width() - width() / 4, 0, width() / 4, height());
	addChild(p);

	Surface_ptr q(new Scrolling);
	content_.reset(new Viewer);
	std::dynamic_pointer_cast<Scrolling>(q)->setScrollable(std::dynamic_pointer_cast<Scrollable>(content_));
	q->init(list_->width(), 0, width() - list_->width() - p->width(), height());
	addChild(q);

	Controller::load(1);
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

