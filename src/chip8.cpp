// chip8.cpp : Defines the entry point for the application.
//

#include "chip8.h"

int main(int argc, char** argv)
{
	VirtualChip8 vm;
	if (argc == 2) {
		vm.LoadCode(argv[1]);
	}
	else {
		std::cout << "Usage: chip8.exe program" << std::endl;
		exit(1);
	}

	vm.display.init();

	for (;;)
	{
		vm.EmulateCycle();

		// Render
		vm.display.nextScreen(vm.gfx);
		

		glfwPollEvents();
	}

	vm.display.End();
	
	return 0;
}
