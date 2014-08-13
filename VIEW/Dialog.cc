#include <vector>
#include "Dialog.h"
#include "Manager.h"
#include <dav/gl.h>

class MsgRenderer
{
	public:
		MsgRenderer(const std::string&, int, int);
		void operator()( ) const;
	private:
		std::vector<std::string> msgs_;
		int w_, h_;
};

Dialog::Dialog(const std::string& msg) : msg_(msg)
{
}

void Dialog::i_setScreen(int w, int h)
{
	setRender(MsgRenderer(msg_, w, h));
}

void Dialog::i_keyPress(Controls key, const modifier_t& m)
{
	if(key == Controls::RETURN) kill();
}

void Dialog::i_suspend(bool s)
{
}

MsgRenderer::MsgRenderer(const std::string& m, int w, int h) : w_(w), h_(h)
{
	int dw = w_ / 2 - h_ / 16;
	int c = dw / (h_ / 16);
	auto i = m.cbegin(), j = m.cbegin();
	while(i != m.cend())
	{
		while(*i == '\n' || *i == ' ' || *i == '\t') ++i;
		while(j < i) ++j;
		for(int d = c ; d > 0 ; --d)
		{
			if(j == m.cend()) break;
			if(*j == '\n') break;
			++j;
		}
		if(i == j) continue;
		msgs_.push_back(std::string(i, j));
		i = j;
	}
}

void MsgRenderer::operator()(void) const
{
	Manager &mngr(Manager::instance());

	gl::draw_rect(0, 0, w_, h_, MXT_BGCOLOR);
	gl::draw_rect(w_ / 4, h_ / 4, w_ * 3 / 4, h_ * 3 / 4, MXT_WHITE);

	int y = h_ / 2 - (msgs_.size() * (h_ / 16 + 1)) / 2;
	int x = w_ / 2;
	for(const auto& s : msgs_)
	{
		mngr.renderString(s, x - mngr.getStringWidth(s) / 2, y, MXT_BLACK);
		y += h_ / 16 + 1;
	}
}

