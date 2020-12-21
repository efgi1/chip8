#include "VirtualChip8.h"

VirtualChip8::VirtualChip8() {
	V = new unsigned char[NO_REGS];
	I = 0;
	pc = 0;
	sp = 0;

	key = new unsigned char[NO_INPUTS];
	gfx = new unsigned char[MAX_WIDTH * MAX_HEIGHT];
	display = Display();
	delay_timer = 0;
	sound_timer = 0;
	
	// Set the fontset
	unsigned short length = *(&chip8_fontset + 1) - chip8_fontset;
	for (int i = 0; i < length; ++i)
		mem.fontset[i] = chip8_fontset[i];
	

	// Clear display
	for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
		gfx[i] = 0;

}

VirtualChip8::~VirtualChip8() {
	delete[] mem.ENTIRE;
	delete[] V;
	delete[] key;
	delete[] gfx;
}

// Reads the file and stores it in mem.code
void VirtualChip8::LoadGame(std::string filename) {
	std::ifstream f(filename, std::ios::binary | std::ios::in);
	if (!f.is_open()) {
		std::cerr << "Coud not open file." << std::endl;
		exit(1);
	}
	char byte;
	unsigned int count = 0;
	while (f.get(byte)) {
		mem.code[count] = byte;
		count++;
	}
}

// Fetch, Decode, Execute are each one tick
void VirtualChip8::EmulateCycle() {
	// Fetch (NOTE codes stored in big endian)
	tick();
	unsigned short big, opcode;
	big = *((unsigned short*)(mem.code + pc));
	opcode = _byteswap_ushort(big);

	//Decode (switch) Execute (case)
	tick();
	switch (opcode) {
	case 0x00EE:
		exit(0);
		break;
	default:
		std::cout << opcode;
		break;
	}
	pc++;
	tick();
}

// Regulates the speed of the emulator
void VirtualChip8::tick() {
	Sleep(HZ);
}
