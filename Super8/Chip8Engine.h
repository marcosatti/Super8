#pragma once

#include <cstdint>
#include <string>
#include <cstdbool>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Chip8TimersEngine.h"
#include "Chip8StackEngine.h"
#include "Chip8KeyEngine.h"

#define NUM_V_REG 16 // 16 8-bit data registers from V0 -> VF
#define NUM_KEYS_KEYPAD 16 // 16 valid keys from 0 -> F (hex)

#define MEMORY_SZ 4096 // 4K of RAM (0x0000 -> 0x0FFF accessable)
#define GFX_MEMORY_SZ 2048 // 2K of VRAM (64 x 32)
#define GFX_XRES 64
#define GFX_YRES 32

#define NUM_BITS_PER_BYTE 8

struct C8_CPU {
	uint16_t pc; // 16-bit wide program counter, contains current address being executed
	uint8_t V[NUM_V_REG]; // 16 8-bit data registers, named from V0 -> VF, VF doubles as a carry flag
	uint16_t I; // 16-bit Address register, used with opcodes that involve memory operations
};

// Forward Decl.
class Chip8TimersEngine;

class Chip8Engine {
public:
	Chip8KeyEngine * key;

	Chip8Engine();
	Chip8Engine(uint8_t * _memory, uint8_t * _gfxmem);
	~Chip8Engine();

	void initialise();

	void loadProgram(std::string path);
	void emulateCycle();

	void setVFRegister(uint8_t num);

	bool checkDrawFlag();
	void setDrawFlag(bool isdraw);

	void setKeyState(uint8_t keyindex, KEY_STATE state);

	void DEBUG_printCPUState();
	void DEBUG_renderGFXText();
	void DEBUG_printSoundTimer();

private:
	uint8_t * memory; // 4096 (0x1000) bytes of memory in total, assumed to be allocated before class initialisation.
	uint8_t * gfxmem; // 2048 (64x32) array containing pixel data (1 or 0) TODO: might be able to change this into a bool array or multiple ints (efficiency)?

	bool drawflag;

	C8_CPU cpu;
	uint16_t opcode; // 16-bit wide opcode holder

	Chip8TimersEngine * timers;
	Chip8StackEngine * stack;

	void allocMem();
	void clearMem();
	void clearGFXMem();
	void clearRegV();
	

	// MSN = most significant nibble (half-byte)
	void handleOpcodeMSN_0();
	void handleOpcodeMSN_1();
	void handleOpcodeMSN_2();
	void handleOpcodeMSN_3();
	void handleOpcodeMSN_4();
	void handleOpcodeMSN_5();
	void handleOpcodeMSN_6();
	void handleOpcodeMSN_7();
	void handleOpcodeMSN_8();
	void handleOpcodeMSN_9();
	void handleOpcodeMSN_A();
	void handleOpcodeMSN_B();
	void handleOpcodeMSN_C();
	void handleOpcodeMSN_D();
	void handleOpcodeMSN_E();
	void handleOpcodeMSN_F();

	void incrementPC();
	void incrementPC(uint8_t bytes);

};

// Note that sprites are 8 - bits wide, while fonts are 4 - bits, so the other 4-bits at the end are padded 0xN0 (0's).
// This is to be copied into the Chip8's memory at runtime.
const uint8_t chip8_fontset[] = { 
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F 
};
#define FONT_WIDTH 5
#define FONTSET_SZ (sizeof(chip8_fontset)/sizeof(chip8_fontset[0]))