// chip8.cpp : Defines the entry point for the application.
//

#include "chip8.h"
#include <chrono>

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
	std::chrono::time_point<std::chrono::system_clock> cpu_time, display_time;
	cpu_time = std::chrono::system_clock::now();
	display_time = std::chrono::system_clock::now();
	while(vm.display.StayOpen())
	{

		std::chrono::duration<double> cpu_elapsed = std::chrono::system_clock::now() - cpu_time;
		if (cpu_elapsed.count() > 1 / 500.)
		{
			vm.EmulateCycle();
			cpu_time = std::chrono::system_clock::now();
		}
		

		std::chrono::duration<double> display_elapsed = std::chrono::system_clock::now() - display_time;
		// Render
		if (display_elapsed.count() > 1 / 60.)
		{
			vm.display.nextScreen(vm.gfx);
			display_time = std::chrono::system_clock::now();
		}
		

		glfwPollEvents();
	}

	vm.display.End();
	
	return 0;
}
