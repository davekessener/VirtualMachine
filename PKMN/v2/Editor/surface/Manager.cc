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
#include "Button.h"
#include "Dialog.h"
#include "../AddMapDlg.h"

#define MXT_ASKSAVE "Do you want to save first?"

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
	list_->init(0, 0, width() / 8, height() - 16);
	addChild(list_);

	Surface_ptr p(new Button("+", [this]
	{
		auto doAdd = [this]{ dialog_(Surface_ptr(new AddMapDlg), 0.333f, 0.75f); };
		if(Controller::isLoaded() && Controller::hasChanged())
		{
			dialog_(Surface_ptr(new Dialog(MXT_ASKSAVE,
			{
				std::make_pair("Yes", [this, doAdd]{ Controller::save(); doAdd(); }),
				std::make_pair("No",  [this, doAdd]{ doAdd(); })
			})), 0.333, 0.333);
		}
		else
		{
			doAdd();
		}
	}));
	p->init(0, height() - 16, list_->width(), 16);
	addChild(p);

	tileset_.reset(new Scrolling);
	tileset_->to<Scrolling>()->setScrollable(std::shared_ptr<Scrollable>(new Tileset));
	tileset_->init(width() - width() / 4, 0, width() / 4, height());

	content_.reset(new Scrolling);
	content_->to<Scrolling>()->setScrollable(std::shared_ptr<Scrollable>(new Viewer));
	content_->init(list_->width(), 0, width() - list_->width() - p->width() - 1, height());

	mc_ = File::end() - File::begin();
}

void Manager::i_doUpdate(int d)
{
	if(Controller::isLoaded() && !content_->hasParent())
	{
		addChild(content_);
		addChild(tileset_);
		dirty();
	}
	else if(!Controller::isLoaded() && content_->hasParent())
	{
		removeChild(content_->ID());
		removeChild(tileset_->ID());
		dirty();
	}

	if(mc_ != File::end() - File::begin())
	{
		mc_ = File::end() - File::begin();
		dirty();
	}
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

void Manager::updateContent(const std::string& s)
{
	std::string map = s.substr(0, s.find_first_of(':'));
	int id = dav::aux::lexical_cast<int>(map);

	if(Controller::isLoaded() && Controller::hasChanged())
	{
		dialog_(Surface_ptr(new Dialog(MXT_ASKSAVE,
			{
				std::make_pair("Yes", [id]{ Controller::save(); Controller::load(id); }),
				std::make_pair("No",  [id]{ Controller::load(id); })
			})), 0.333, 0.333);
	}
	else
	{
		Controller::load(id);
	}
}

}}

