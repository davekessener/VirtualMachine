#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include "ddef.h"
#include "CPU.h"
#include "Instruction.h"
#include "Processor.h"
#include "Screen.h"
#include "Image.h"
#include "SDLException.h"
#include "Logger.h"

class BR
{
	public:
		BR(const std::string&);
		~BR( ) throw();
		inline operator vm::WORD *( ) { return c_; }
		inline int length( ) { return l_; }
	private:
		vm::WORD *c_;
		int l_;
};

void print_opcodes(const std::string&);

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace cpu;
	using namespace sdl;

	std::vector<std::string> args(argv, argv + argc);

	if(args.size() > 2 && args[1] == "--print-opcodes")
	{
		print_opcodes(args[2]);
		return 0;
	}

	assert(args.size() > 1);

	Screen &s = Screen::instance();
	Processor p;
	BR boot(args[1]);

	p.init();
	p.reset();
	p.write(0, boot.length(), boot);

	while(p.isRunning())
	{
		p.execute();
		s.update();
		s.refresh();
	}

	std::ostringstream oss;
	oss << p << std::endl << p.printRAM(0, 0x120) << std::endl << p.printRAM(0xc000, 120*46) << std::endl;
	Logger::log(oss.str());

	return 0;
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

BR::BR(const std::string& fn)
{
	std::ifstream in(fn.c_str(), std::ios::in | std::ios::binary);

	assert(in.is_open());

	in.seekg(0, std::ios_base::end);
	l_ = in.tellg();
	in.seekg(0, std::ios_base::beg);

	c_ = new vm::WORD[l_];

	in.read(reinterpret_cast<char *>(c_), l_);
	l_ <<= 1;
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

