// chip8.cpp : Defines the entry point for the application.
//

#include "chip8.h"

int main(int argc, char** argv)
{
	VirtualChip8 vm;
	vm.Run(argv[1]);
	return 0;
}
