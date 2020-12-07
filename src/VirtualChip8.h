#ifndef VIRTUALCHIP8_H
#define VIRTUALCHIP8_H

#include <iostream>

#define MEM_SIZE 1024 * 4 // 4 KB
#define REGS 16
#define INPUTS 16
#define WIDTH 64
#define HEIGHT 32

struct Memory {
	unsigned char* ENTIRE = new unsigned char[MEM_SIZE];
	unsigned char* interpreter = ENTIRE;							// 0x000 - 0x1FF UNNECESSARY FOR EMULATOR
	unsigned char* code = ENTIRE + 0x200;							// 0x200 - 0xE9F
	unsigned char* stack = ENTIRE + 0xEA0;							// 0xEA0 - 0xEFF
	unsigned char* disp_ref = ENTIRE + 0xF00;						// 0xF00 - 0xFFF
};



class VirtualChip8 {
public:
	VirtualChip8();
	~VirtualChip8();
	void Run(std::string filename);
private:
	Memory mem;
	// VF doubles as a flag in some cases:
	// Addition - Carry flag
	// Subtraction - "no borrow" flag
	// Draw - set upon pixel collision
	unsigned char* V;
	unsigned short I;
	unsigned short pc;
	unsigned short sp;

	unsigned char *key;
	unsigned char *gfx;
	unsigned char delay_timer;
	unsigned char sound_timer;
	

};

#endif