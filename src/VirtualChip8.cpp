#include "VirtualChip8.h"

VirtualChip8::VirtualChip8() {

	srand(std::time(nullptr));
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
	unsigned short big, inst, opcode, x, y, kk, nnn, n;
	bool collision;
	const unsigned short FONTSET_SPRITE_SIZE = 10;
	big = *((unsigned short*)(mem.code + pc));
	pc++;

	inst = _byteswap_ushort(big);

	/*nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
	n or nibble - A 4-bit value, the lowest 4 bits of the instruction
	x - A 4-bit value, the lower 4 bits of the high byte of the instruction
	y - A 4-bit value, the upper 4 bits of the low byte of the instruction
	kk or byte - An 8-bit value, the lowest 8 bits of the instruction*/
	opcode = (inst & 0xF000) >> 12u;
	nnn = inst & 0x0FFF;
	n = inst & 0x000F;
	x = (inst & 0x0F00) >> 8u;
	y = (inst & 0x00F0) >> 4u;
	kk = inst & 0x00FF;

	//Decode (switch) Execute (case)
	tick();
	
	switch (opcode) {
	case 0x0:
		if (inst == 0x00E0) {
			for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i) {
				gfx[i] = 0;
			}
		}
		else if (inst == 0x00EE) {
			exit(0);
		}
		break;
	case 0x1:
		pc = inst & 0x0FFF;
		break;
	case 0x2:
		sp++;
		*((unsigned int*)(mem.stack) + sp) = pc;
		pc = nnn;
		break;
	case 0x3:
		if (V[x] == kk)
			pc++;
		break;
	case 0x4:
		if (V[x] != kk)
			pc++;
		break;
	case 0x5:
		if (V[x] == V[y])
			pc++;
		break;
	case 0x6:
		V[x] = kk;
		break;
	case 0x7:
		V[x] = V[x] + kk;
	case 0x8:
		switch (n) {
		case 0x1:
			V[x] = V[x] | V[y];
			break;
		case 0x2:
			V[x] = V[x] & V[y];
			break;
		case 0x3:
			V[x] = V[x] ^ V[y];
			break;
		case 0x4:
			V[0xF] = (V[x] + V[y]) > 255;
			V[x] = (V[x] + V[y]) & 0x00FF;
			break;
		case 0x5:
			V[0xF] = V[x] > V[y];
			V[x] -= V[y];
			break;
		case 0x6:
			V[0xF] = V[x] & 0x0001;
			V[x] /= 2;
			break;
		case 0x7:
			V[0xF] = V[y] > V[x];
			V[x] = V[y] - V[x];
			break;
		case 0xE:
			V[0xF] = V[x] >> 15u;
			V[x] *= 2;
			break;
		default:
			break;
		}
		break;
	case 0x9:
		if (V[x] != V[y])
			pc++;
		break;
	case 0xA:
		I = nnn;
		break;
	case 0xB:
		pc = V[0] + nnn;
		break;
	case 0xC:
		V[x] = (std::rand() % 255) & kk;
		break;
	case 0xD:
		collision = false;
		for (int i = 0; i < n; ++i) {
			if (gfx[V[x] * V[y]] == *(mem.fontset + I + i))
				collision = true;
			gfx[V[x] * V[y]] ^= *(mem.fontset + I + i);
		}
		V[0xF] = collision;
		break;
	case 0xE:
		// TODO add processing input
		switch (kk) {
		case 0x9E:
			break;
		case 0xA1:
			break;
		}
		break;
	case 0xF:
		switch (kk) {
		case 0x07:
			V[x] = delay_timer;
			break;
		case 0x0A:
			// TODO key press
			break;
		case 0x15:
			delay_timer = V[x];
			break;
		case 0x18:
			sound_timer = V[x];
			break;
		case 0x1E:
			I += V[x];
			break;
		case 0X29:
			I = V[x] * FONTSET_SPRITE_SIZE * sizeof(unsigned short);
			break;
		case 0x33:
			*(mem.ENTIRE + I) = V[x] & 0x000F;
			*(mem.ENTIRE + I + 1) = (V[x] & 0x00F0) >> 4u;
			*(mem.ENTIRE + I + 2) = (V[x] & 0x0F00) >> 8u;
			*(mem.ENTIRE + I + 3) = (V[x] & 0xF000) >> 12u;
			break;
		case 0x55:
			for (int i = 0; i < NO_REGS; ++i)
				*((unsigned short*)(mem.ENTIRE + I) + i) = V[i];
			break;
		case 0x65:
			for (int i = 0; i < NO_REGS; ++i)
				V[i] = * ((unsigned short*)(mem.ENTIRE + I) + i);
			break;
		}
		break;
	default:
			std::cout << inst;
			// std::cerr << "Invalid instruction: Something went wrong." << std::endl;
			// std::exit(1);
	}
	
	tick();
}

// Regulates the speed of the emulator
void VirtualChip8::tick() {
	Sleep(HZ);
}
