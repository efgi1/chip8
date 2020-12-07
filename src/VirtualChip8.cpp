#include "VirtualChip8.h"


VirtualChip8::VirtualChip8() {
	V = new unsigned char[REGS];
	I = 0;
	pc = 0x200;
	sp = 0;

	key = new unsigned char[INPUTS];
	gfx = new unsigned char[WIDTH * HEIGHT];
	delay_timer = 0;
	sound_timer = 0;
}
VirtualChip8::~VirtualChip8() {
	delete[] mem.ENTIRE;
	delete[] V;
	delete[] key;
	delete[] gfx;
}
void VirtualChip8::Run(std::string filename) {
	std::cout << "Hello, vm" << std::endl;
}
