#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ddef.h"
#include "CPU.h"
#include "Instruction.h"
#include "Processor.h"
#include "Screen.h"
#include "Image.h"
#include "SDLException.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace cpu;
	using namespace sdl;

	std::vector<std::string> args(argv, argv + argc);

	Screen &s = Screen::instance();
	Processor p;
	WORD code[] = {
		0x01, 0x01, 35,
		0x01, 0x02, 0xc000,
	//loop
		0x03, 0x03, 0x01,
		0x11, 0x03, 21,
		0x04, 0x02, 0x03,
		0x05, 0x01,
		0x05, 0x02,
		0x10, 6,
	//end
		0x01, 0x01, 0x01,
		0x18, 0x00, 0x01,
		0x01, 0x01, 0xc000,
		0x18, 0x00, 0x01,
	//here
		0x10, 33 + 16,
	//text
		'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0,
	//49
		0x01, 0x01, 0x0101,
		0x01, 0x02, 67,
		0x04, 0x01, 0x02,
		0x01, 0x01, 0x01,
		0x18, 0x01, 0x01,
		0x19,
	//here 65
		0x10, 65,
	//routine 67
		0x17, 0x01, 0x01,
		0x01, 0x02, 'q',
		0x0e, 0x01, 0x02,
		0x11, 0x01, 80,
		0x14,
	//quit 80
		0x00
	};

	p.init();
	p.reset();
	p.write(0, sizeof(code)/sizeof(*code), code);

	while(p.isRunning())
	{
		p.execute();
		s.update();
		s.refresh();
	}

	std::ostringstream oss;
	oss << p << std::endl << p.printRAM(0, 0x120) << std::endl;
	Logger::log(oss.str());

	std::ofstream manual("ins.txt");

	if(manual.is_open())
	{
		manual << p.printOpcodes() << std::endl;
		manual.close();
	}

	return 0;
}

