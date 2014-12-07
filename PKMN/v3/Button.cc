#include "Button.h"
#include "Icon.h"
#include "Label.h"
#include "TextStitcher.h"

namespace dav { namespace pkmn { namespace screen {

void Button::loadIcon(const std::string& s)
{
	removeContent();
	addChild(content_ = std::make_shared<Icon>(s));
	text_info i = TextureStitcher::LoadIcon(s);
	uint w(width()), h(height());
	content_->init((w - i.size) / 2, (h - i.size) / 2, i.size, i.size);
	dirty(true);
}

void Button::loadText(const std::string& s)
{
	uint w(width()), h(height());
	removeContent();
	addChild(content_ = std::make_shared<Label>(s));
	content_->init(0, 0, w, h);
	Label_ptr p = std::dynamic_pointer_cast<Label>(content_);
	uint pw(p->labelWidth()), ph(p->labelHeight());
	p->move((w - pw) / 2, (h - ph) / 2);
	p->resize(pw, ph);
	dirty(true);
}

void Button::removeContent(void)
{
	if(content_)
	{
		removeChild(content_->ID());
		content_.reset();
	}
}

void Button::press(bool d)
{
	if(!active_) d = false;

	if(d != pressed_)
	{
		pressed_ = d;
		if(content_) content_->move(content_->X() + (d ? 1 : -1), content_->Y() + (d ? 1 : -1));
		dirty(true);
	}
}

void Button::i_doInit(void)
{
	up_ = TextureStitcher::LoadIcon(MXT_ICON_BASEUP);
	down_ = TextureStitcher::LoadIcon(MXT_ICON_BASEDOWN);
	pressed_ = false;
	active_ = false;
}

void Button::i_doMouseDown(MouseButtons b, int x, int y)
{
	press(active_ = true);
}

void Button::i_doMouseUp(MouseButtons b, int x, int y)
{
	press(active_ = false);
}

void Button::i_doMouseDrag(int x, int y)
{
	press(isOver(x, y));
}

void Button::i_doRender(void)
{
	int w(width()), h(height());

	text_info ti(pressed_ ? down_ : up_);
	float d = (ti.u2 - ti.u1) / 2, n = ti.size / 2;

	draw(ti.id, ti.u1, ti.v1, ti.u1 + d, ti.v1 + d, 0, 0, n, n);
	draw(ti.id, ti.u1 + d, ti.v1, ti.u2, ti.v1 + d, w - n, 0, w, n);
	draw(ti.id, ti.u1, ti.v1 + d, ti.u1 + d, ti.v2, 0, h - n, n, h);
	draw(ti.id, ti.u1 + d, ti.v1 + d, ti.u2, ti.v2, w - n, h - n, w, h);

	draw(ti.id, ti.u1 + d / 2, ti.v1, ti.u2 - d / 2, ti.v1 + d, n, 0, w - n, n);
	draw(ti.id, ti.u1 + d / 2, ti.v2 - d, ti.u2 - d / 2, ti.v2, n, h - n, w - n, h);
	draw(ti.id, ti.u1, ti.v1 + d / 2, ti.u1 + d, ti.v2 - d / 2, 0, n, n, h - n);
	draw(ti.id, ti.u2 - d, ti.v1 + d / 2, ti.u2, ti.v2 - d / 2, w - n, n, w, h - n);

	draw(ti.id, ti.u1 + d / 2, ti.v1 + d / 2, ti.u2 - d / 2, ti.v2 - d / 2, n, n, w - n, h - n);
}

Surface_ptr Button::i_doGetControlAt(int x, int y)
{
	return shared_from_this();
}

}}}

