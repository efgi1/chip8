#include "VirtualChip8.h"

VirtualChip8::VirtualChip8() {

	srand(std::time(nullptr));
	V = new unsigned char[NO_REGS];
	I = 0;
	pc = 0x200;
	sp = 1;
	key = new unsigned char[NO_INPUTS];
	gfx = new unsigned char[MAX_WIDTH * MAX_HEIGHT];
	display = Display();
	input = new Input;
	soundEngine = irrklang::createIrrKlangDevice();
	sound = soundEngine->play2D("C:\\Users\\nateh\\Desktop\\Projects\\chip8\\tone.wav", false, true);
	delay_timer = 0;
	sound_timer = 0;
	
	//clear Registers
	for (int i = 0; i < NO_REGS; ++i)
		V[i] = 0;

	// Set the fontset
	unsigned int length = 0x1FF;
	for (unsigned int i = 0; i < length; ++i)
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
	delete input;
	sound->drop();
}

// Reads the file and stores it in mem.code
void VirtualChip8::LoadCode(std::string filename) {
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read((char*)mem.code, size);
		file.close();
	}
}

// Fetch, Decode, Execute are each one tick
void VirtualChip8::EmulateCycle() {
	const unsigned short FONTSET_SPRITE_SIZE = 5;

	//tick();
	unsigned char x, y, kk, n;
	unsigned short big, inst, opcode, nnn, sprite;
	bool collision;
	

	// Fetch (NOTE codes stored in big endian)
	big = *(unsigned short*)(mem.ENTIRE + pc);
	inst = _byteswap_ushort(big);
	//std::cout << std::hex << "pc: " << pc << std::endl;
	pc+=2;
	

	
	//std::cout << inst << std::endl;

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
	//tick();
	
	if (delay_timer > 0)
		--delay_timer;
	if (sound_timer > 0) {
		if(sound->getIsPaused())
			sound->setIsPaused(false);
		--sound_timer;
	}
	else if (sound_timer == 0 && !sound->getIsPaused()) {
		sound->setIsPaused(true);
	}

	input->ProcessInput(display.window);

	switch (opcode) {
	case 0x0:
		if (inst == 0x00E0) {
			for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i) {
				gfx[i] = 0;
			}
		}
		else if (inst == 0x00EE) {
			pc = *((unsigned short*)(mem.stack) - sp);
			sp++;
		}
		break;
	case 0x1:
		pc = nnn;
		break;
	case 0x2:
		sp--;
		*((unsigned short*)(mem.stack) - sp) = pc;
		pc = nnn;
		break;
	case 0x3:
		if (V[x] == kk)
			pc+=2;
		break;
	case 0x4:
		if (V[x] != kk)
			pc+=2;
		break;
	case 0x5:
		if (V[x] == V[y])
			pc+=2;
		break;
	case 0x6:
		V[x] = kk;
		break;
	case 0x7:
		V[x] += kk;
		break;
	case 0x8:
		switch (n) {
		case 0x0:
			V[x] = V[y];
			break;
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
			V[x] = (V[x] + V[y]);
			break;
		case 0x5:
			V[0xF] = V[x] >= V[y];
			V[x] -= V[y];
			break;
		case 0x6:
			V[0xF] = V[x] & 0x01;
			V[x] /= 2;
			break;
		case 0x7:
			V[0xF] = V[y] >= V[x];
			V[x] = V[y] - V[x];
			break;
		case 0xE:
			V[0xF] = V[x] >> 7u;
			V[x] *= 2;
			break;
		default:
			break;
		}
		break;
	case 0x9:
		if (V[x] != V[y])
			pc+=2;
		break;
	case 0xA:
		I = nnn;
		break;
	case 0xB:
		pc = (V[0] + nnn);
		break;
	case 0xC:
		V[x] = (std::rand() % 255) & kk;
		break;
	case 0xD:
		collision = false;
		for (short yLine = 0; yLine < n && (yLine + V[y]) < MAX_HEIGHT; ++yLine) {
			sprite = mem.ENTIRE[I + yLine];
			for (short xLine = 0; xLine < 8; ++xLine) {
				if (((sprite >> (7 - xLine)) & 1) == 1) {
					if (gfx[(V[x] + xLine) + ((V[y] + yLine) * 64)] == 1)
						collision = true;
					gfx[(V[x] + xLine) + ((V[y] + yLine) * 64)] ^= 1;
				}
			}
		}
		V[0xF] = collision;
		break;
	case 0xE:
		// TODO add processing input
		switch (kk) {
		case 0x9E:
			if (input->WasPressed(display.window, V[x])) {
				pc += 2;
			}
			break;
		case 0xA1:
			if (!input->WasPressed(display.window, V[x])) {
				pc += 2;
			}
			break;
		}
		break;
	case 0xF:
		switch (kk) {
		case 0x07:
			V[x] = delay_timer;
			break;
		case 0x0A:
			V[x] = input->WaitForInput(display.window);
			break;
		case 0x15:
			delay_timer = V[x];
			break;
		case 0x18:
			sound_timer = V[x];
			break;
		case 0x1E:
			I += V[x];
			V[0xF] = I > 0xFFF;
			break;
		case 0X29:
			I = V[x] * FONTSET_SPRITE_SIZE;
			break;
		case 0x33:
			mem.ENTIRE[I] = V[x] / 100;
			mem.ENTIRE[I + 1] = (V[x] / 10) % 10;
			mem.ENTIRE[I + 2] = V[x] % 10;
			break;
		case 0x55:
			for (int i = 0; i <= x; i++)
				mem.ENTIRE[I + i] = V[i];
			break;
		case 0x65:
			for (int i = 0; i <= x; i++)
				V[i] = mem.ENTIRE[I + i];
			break;
		}
		break;
	default:
			std::cout << inst;
			std::cerr << "Invalid instruction: Something went wrong." << std::endl;
			std::exit(1);
	}
	
	//tick();
}

// Regulates the speed of the emulator
void VirtualChip8::tick() {
	Sleep(HZ);
}

