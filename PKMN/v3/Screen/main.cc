#include <iostream>
#include "Manager.h"

namespace dav { namespace pkmn {

using screen::Surface_ptr;

class Test : public screen::Surface
{
	public:
	private:
		void i_doInit( ) { }
		void i_doUpdate(uint) { }
		void i_doRender( )
		{
			auto s(getSize());
			fillRect(0, 0, s.w, s.h, 0xff0000);
			drawString("This is a Test!", -8, -8, 0, 16);
		}
		void i_doResize( ) { }
};

class Root : public screen::Surface
{
	public:
	private:
		void i_doInit( )
		{
			addChild(test_ = std::make_shared<Test>());
			test_->init(256, 256, 168, 16);
		}
		void i_doUpdate(uint) { }
		void i_doRender( )
		{
			auto s(getSize());
			fillRect(0, 0, s.w, s.h, 0xffffff);
		}
		void i_doResize( ) { }
	private:
		Surface_ptr test_;
};

}}

int main(int argc, char *argv[])
try
{
	using namespace dav::pkmn;
	using namespace dav::pkmn::screen;

	runProgram("Test", std::make_shared<Root>());

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

