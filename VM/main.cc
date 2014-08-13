#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include "ddef.h"
#include "CPU.h"
#include "Instruction.h"
#include "Processor.h"
//#include "Image.h"
//#include "SDLException.h"
#include "Logger.h"
#include "Timer.h"
#include "Screen.h"
#include <dav/sdl.h>
#include <dav/gl.h>

#define MXT_P 4

class BR
{
	public:
		~BR( ) throw();
		void load(const std::string&);
		inline operator WORD *( ) { return c_; }
		inline int length( ) { return l_; }
	private:
		WORD *c_;
		int l_;
};

typedef unsigned int uint;
void do_init(int, int);
bool do_update(int);
void do_keyboard(::sdl::Controls, bool);
void do_mouse(uint, uint, int, int);

void print_opcodes(const std::string&);

vm::cpu::Processor p;
BR boot;
Timer t;

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace cpu;
	using namespace vga;

	std::vector<std::string> args(argv, argv + argc);

	if(args.size() > 2 && args[1] == "--print-opcodes")
	{
		print_opcodes(args[2]);
		return 0;
	}

	assert(args.size() > 1);

//	Screen &s = Screen::instance();

	boot.load(argv[1]);

	::sdl::set_init(&do_init);
	::sdl::set_update(&do_update);
	::sdl::set_input(&do_keyboard, &do_mouse);

	::sdl::start("CPU", COLS * CHAR_SIZE, ROWS * (CHAR_SIZE + MXT_P));

	std::ostringstream oss;
	oss << p << std::endl 
		<< p.printRAM(0, 0x120) << std::endl 
		<< p.printRAM(0x300, 0x300) << std::endl 
		<< p.printRAM(0xc000, 120*46) << std::endl;
	Logger::log(oss.str());

	return 0;
}

void do_init(int w, int h)
{
	gl::init2d(w, h);

	p.init();
	p.reset();
	p.write(0, boot.length(), boot);
}

bool do_update(int d)
{
	t.reset();
	const int DELTA = 1000 / 60; // 1 / FPS = T per frame
	while(t.elapsed() < DELTA && !p.isSuspended())
	{
		for(int i = 16 ; i ; --i)
		{
			p.execute();
		}
	}
	p.step();

	gl::start_draw();
	
	vm::sdl::Screen::instance().render();
	
	gl::update();

	return p.isRunning();
}

void do_keyboard(::sdl::Controls c, bool down)
{
	if(down)
	{
		if(c == ::sdl::Controls::ESCAPE) p.halt();
		vm::sdl::Screen::instance().pressKey(c);
	}
}

void do_mouse(uint x, uint y, int dx, int dy)
{
}

void print_opcodes(const std::string& fn)
{
	std::ofstream manual(fn.c_str());

	if(manual.is_open())
	{
		vm::cpu::Processor p;
		p.init();
		manual << p.printOpcodes() << std::endl;
		manual.close();
	}
}

void BR::load(const std::string& fn)
{
	std::ifstream in(fn.c_str(), std::ios::in | std::ios::binary);

	assert(in.is_open());

	in.seekg(0, std::ios_base::end);
	l_ = in.tellg();
	in.seekg(0, std::ios_base::beg);

	c_ = new WORD[l_ / 2];

	in.read(reinterpret_cast<char *>(c_), l_);
	l_ /= 2;
}

BR::~BR(void) throw()
{
	delete[] c_;
}

//	WORD code[] = {
//		0x01, 0x01, 35,
//		0x01, 0x02, 0xc000,
//	//loop
//		0x03, 0x03, 0x01,
//		0x04, 0x02, 0x03,
//		0x11, 0x03, 21,
//		0x05, 0x01,
//		0x05, 0x02,
//		0x10, 6,
//	//end
//		0x01, 0x01, 0x01,
//		0x18, 0x00, 0x01,
//		0x01, 0x01, 0xc000,
//		0x18, 0x00, 0x01,
//	//here
//		0x10, 33 + 16,
//	//text
//		'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0,
//	//49
//		0x01, 0x01, 0x0101,
//		0x01, 0x02, 67,
//		0x04, 0x01, 0x02,
//		0x01, 0x01, 0x01,
//		0x18, 0x01, 0x01,
//		0x19,
//	//here 65
//		0x10, 65,
//	//routine 67
//		0x17, 0x01, 0x01,
//		0x01, 0x02, 'q',
//		0x0e, 0x01, 0x02,
//		0x11, 0x01, 80,
//		0x14,
//	//quit 80
//		0x00
//	};

