#include <iostream>
#include "CPU.h"
using namespace std;

int main(int argc, char *argv[])
{
	CPU cpu;
	
	WORD b[] = {0, 0, 12, 0xd, 0, 7, 0, 10, 5, 3, 0xb};
	
	cpu.writeMemory(b, 0, 12);
	
	while(!cpu.isHalted())
	{
		cpu.printNextInstruction();
		cpu.execute();
	}
	
	cpu.printRegisters();
	
	return 0;
}

