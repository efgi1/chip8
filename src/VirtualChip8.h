#ifndef VIRTUALCHIP8_H
#define VIRTUALCHIP8_H

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>
#include <Windows.h>
#include <intrin.h>

#include "Display.h"

#define MEM_SIZE 1024 * 4 // 4 KB
#define NO_REGS 16
#define NO_INPUTS 16
#define MAX_WIDTH 64
#define MAX_HEIGHT 32
#define HZ 60 / 1000 // Speed at which the system runs in cycles per millisecond

const unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct Memory {
	unsigned char* ENTIRE = new unsigned char[MEM_SIZE];
	unsigned char* fontset = ENTIRE;								// 0x000 - 0x1FF
	unsigned char* code = ENTIRE + 0x200;							// 0x200 - 0xE9F
	unsigned char* stack = ENTIRE + 0xEA0;							// 0xEA0 - 0xEFF
	unsigned char* disp_ref = ENTIRE + 0xF00;						// 0xF00 - 0xFFF
};



class VirtualChip8 {
public:
	VirtualChip8();
	~VirtualChip8();
	void LoadGame(std::string filename);
	void EmulateCycle();
	Display display;
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
	
	void tick();

};

#endif