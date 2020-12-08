#include "VirtualChip8.h"
#include <fstream>
#include <intrin.h>


VirtualChip8::VirtualChip8() {
	V = new unsigned char[REGS];
	I = 0;
	pc = 0;
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

void VirtualChip8::LoadGame(std::string filename) {
	// Read file and store into memory
	std::ifstream f(filename, std::ios::binary | std::ios::in);
	if (!f.is_open()) {
		std::cerr << "Coud not open file." << std::endl;
		exit(1);
	}
	char byte;
	unsigned int count = 0;
	while (f.get(byte)) {
		*(mem.code + count) = byte;
		count++;
	}
}

void VirtualChip8::EmulateCycle() {
	unsigned short big, opcode;
	big = *((unsigned short*)(mem.code + pc));
	opcode = _byteswap_ushort(big);
	switch (opcode) {
	case 0x00EE:
		exit(0);
		break;
	default:
		std::cout << opcode;
		break;
	}
	pc++;
}
