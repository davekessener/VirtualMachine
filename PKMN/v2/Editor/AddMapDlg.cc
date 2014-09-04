#include "AddMapDlg.h"
#include "surface/Label.h"
#include "surface/Button.h"
#include "surface/StringInput.h"
#include "Text.h"
#include "File.h"
#include "Controller.h"
#include <dav/gl.h>
#include <dav/utils.h>
#include <aux>

namespace editor
{
	void AddMapDlg::i_doInit(void)
	{
		using surface::Surface_ptr;
		using surface::Label;
		using surface::Button;
		using surface::StringInput;
		using dav::aux::lexical_cast;

		int x = width() / 4, y = 16, w;

		Label *l = new Label("ID:");
		w = l->getEstWidth();
		Surface_ptr p(l);
		p->init(x - w - 8, y, w, Text::C_W * 2);
		addChild(p);

		id_in_.reset(new StringInput);
		id_in_->init(x + 8, y, (width() - x) - 24, Text::C_W * 2);
		addChild(id_in_);

		y += 3 * Text::C_W;

		l = new Label("Name:");
		w = l->getEstWidth();
		p.reset(l);
		p->init(x - w - 8, y, w, Text::C_W * 2);
		addChild(p);

		name_in_.reset(new StringInput);
		name_in_->init(x + 8, y, (width() - x) - 24, Text::C_W * 2);
		addChild(name_in_);

		y += 3 * Text::C_W;

		l = new Label("Width:");
		w = l->getEstWidth();
		p.reset(l);
		p->init(x - w - 8, y, w, Text::C_W * 2);
		addChild(p);

		width_in_.reset(new StringInput);
		width_in_->init(x + 8, y, (width() - x) - 24, Text::C_W * 2);
		addChild(width_in_);

		y += 3 * Text::C_W;

		l = new Label("Height:");
		w = l->getEstWidth();
		p.reset(l);
		p->init(x - w - 8, y, w, Text::C_W * 2);
		addChild(p);

		height_in_.reset(new StringInput);
		height_in_->init(x + 8, y, (width() - x) - 24, Text::C_W * 2);
		addChild(height_in_);

		p.reset(new Button("OK", [this]{ create(); }));
		p->init(16, height() - 24, width() / 2 - 24, 16);
		addChild(p);

		p.reset(new Button("Cancel", [this]{ hide(); }));
		p->init(width() / 2 + 8, height() - 24, width() / 2 - 24, 16);
		addChild(p);

		id_in_->to<StringInput>()->setInput(lexical_cast<std::string>(File::getNextID()));

		Focus(name_in_->ID());
	}

	void AddMapDlg::i_doRender(void) const
	{
		point p(getAbsCoords()), q(p.x + width(), p.y + height());

		dav::gl::fill_rect(p.x, p.y, q.x, q.y, 0x404040);
		dav::gl::fill_rect(p.x + 2, p.y + 2, q.x - 2, q.y - 2, 0xc0c0c0);
	}

	void AddMapDlg::create(void)
	{
		using surface::StringInput;
		using dav::aux::lexical_cast;
		using dav::utils::toInt;

		std::string id_s = id_in_->to<StringInput>()->getInput();
		std::string name = name_in_->to<StringInput>()->getInput();
		std::string width_s = width_in_->to<StringInput>()->getInput();
		std::string height_s = height_in_->to<StringInput>()->getInput();

		if(id_s.empty()) id_s = lexical_cast<std::string>(File::getNextID());

		if(name.empty()) throw std::string("'Name' cannot be empty!");
		if(width_s.empty()) throw std::string("'Width' cannot be empty!");
		if(height_s.empty()) throw std::string("'Height' cannot be empty!");

		int id = toInt(id_s), w = toInt(width_s), h = toInt(height_s);

		if(id < 0) throw std::string("Negative IDs are not valid!");
		if(w <= 0 || h <= 0) throw std::string("Width and height must be positive!");

		id = File::insert(id, name, w, h);
		Controller::load(id);

		hide();
	}
}

