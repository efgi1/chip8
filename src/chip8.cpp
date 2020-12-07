// chip8.cpp : Defines the entry point for the application.
//

#include "chip8.h"

int main(int argc, char** argv)
{
	VirtualChip8 vm;
	if (argc == 2) {
		vm.Run(argv[1]);
	}
	else {
		std::cout << "Usage: chip8.exe program" << std::endl;
	}
	return 0;
}
