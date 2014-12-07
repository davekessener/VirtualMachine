#include <iostream>
#include <unistd.h>
#include "PNGLoader.h"
#include "Manager.h"
#include "Label.h"
#include "Icon.h"
#include "Button.h"

namespace dav { namespace pkmn {

using screen::Surface_ptr;

class Test : public screen::Surface
{
	using screen::Surface::MouseButtons;
	public:
	private:
		void i_doInit( )
		{
			p_.x = p_.y = last_.x = last_.y = 0;
		}
		void i_doRender( )
		{
			auto s(getSize());
			fillRect(0, 0, s.w, s.h, 0);
			fillRect(1, 1, s.w - 1, s.h - 1, 0xffffff);
			fillRect(p_.x - 5, p_.y - 5, p_.x + 5, p_.y + 5, 0xff0000);
		}
		void i_doMouseHover(int x, int y)
		{
			if(x != last_.x || y != last_.y)
			{
				p_.x = x + (x - last_.x);
				p_.y = y + (y - last_.y);
				last_.x = x; last_.y = y;
				dirty(true);
			}
		}
		coords p_, last_;
};

class Visualizer : public screen::Label
{
	private:
		void i_doRender(void)
		{
			int w(width()), h(height());
			fillRect(0, 0, w, h, 0x000000);
			fillRect(1, 1, w - 1, h - 1, 0xffffff);
			screen::Label::i_doRender();
		}
};

class Root : public screen::Surface
{
	public:
	private:
		void i_doInit( )
		{
			addChild(test_ = std::make_shared<Test>());
			test_->init(256, 256, 500, 500);

			addChild(lbl_ = std::make_shared<Visualizer>(), 20);
			lbl_->init(8, 8, 80, 80);
			lbl_->message("This is a test!");
			lbl_->center(true);
			lbl_->spacing(lbl_->charsize());
			
			addChild(icon_ = std::make_shared<screen::Icon>(), 10);
			icon_->init(160, 160, 16, 16);
			icon_->load("icons/save.png");

			addChild(button_ = std::make_shared<screen::Button>());
			button_->init(100, 8, 64, 32);
			button_->loadIcon("icons/split.png");
			button_->loadText("OK");
			
			id = screen::PNGLoader::LoadPNG("test.png");
		}
		void i_doRender( )
		{
			auto s(getSize());
			fillRect(0, 0, s.w, s.h, 0xffffff);
			draw(id, 0, 0, 1, 1, 800, 100, 832, 132);
		}
	private:
		DWORD id;
		Surface_ptr test_;
		screen::Label_ptr lbl_;
		screen::Icon_ptr icon_;
		screen::Button_ptr button_;
};

}}

int main(int argc, char *argv[])
try
{
	using namespace dav::pkmn;
	using namespace dav::pkmn::screen;

	chdir("resource/");

	runProgram("Test", std::make_shared<Root>());

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

