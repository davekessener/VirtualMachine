#include <iostream>
#include <unistd.h>
#include "PNGLoader.h"
#include "Manager.h"
#include "Label.h"
#include "Icon.h"

namespace dav { namespace pkmn {

using screen::Surface_ptr;

class Test : public screen::Surface
{
	using screen::Surface::MouseButtons;
	public:
	private:
		void i_doInit( )
		{
			p_.x = p_.y = 0;
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
			if(x != p_.x || y != p_.y)
			{
				p_.x = x; p_.y = y;
				dirty(true);
			}
		}
		coords p_;
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
			
			id = screen::PNGLoader::LoadPNG("test.png");
			cs = screen::PNGLoader::LoadPNG("charset.png");
		}
		void i_doRender( )
		{
			auto s(getSize());
			fillRect(0, 0, s.w, s.h, 0xffffff);
			draw(id, 0, 0, 1, 1, 800, 100, 832, 132);
			draw(cs, 0.0625, 0, 0.125, 0.0625, 800, 64, 832, 96);
		}
	private:
		DWORD id, cs;
		Surface_ptr test_;
		screen::Label_ptr lbl_;
		screen::Icon_ptr icon_;
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

