// chip8.cpp : Defines the entry point for the application.
//

#include "chip8.h"
#include <windows.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
	VirtualChip8 vm;
	if (argc == 2) {
		vm.LoadGame(argv[1]);
	}
	else {
		std::cout << "Usage: chip8.exe program" << std::endl;
		exit(1);
	}
	vm.display.init();
	for (;;)
	{
		vm.EmulateCycle();

		// Render Loop
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		vm.display.nextScreen();
		glfwPollEvents();
	}

	
	return 0;
}
