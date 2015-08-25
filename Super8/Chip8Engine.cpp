#include "stdafx.h"
#include "Chip8Engine.h"


Chip8Engine::Chip8Engine() {
	allocMem();
}

Chip8Engine::Chip8Engine(uint8_t * _memory, uint8_t * _gfxmem) {
	memory = _memory;
	gfxmem = _gfxmem;
}


Chip8Engine::~Chip8Engine() {
	delete[] memory;
	delete[] gfxmem;
	delete key;
	delete stack;
	delete timers;
}

void Chip8Engine::allocMem() {
	// Allocate main memory and gfx memory
	memory = new uint8_t[MEMORY_SZ];
	gfxmem = new uint8_t[GFX_MEMORY_SZ];
}

void Chip8Engine::initialise() {
	// Initialise & reset timers
	timers = new Chip8TimersEngine(this);
	stack = new Chip8StackEngine();
	key = new Chip8KeyEngine();

	cpu.pc = (uint16_t)0x200;					// Program counter starts at 0x200
	opcode = (uint16_t)0x0000;					// Reset current opcode	
	cpu.I = (uint16_t)0x000;					// Reset index register
	stack->resetStack();						// Reset stack pointer

	clearGFXMem();								// Clear display	
	clearRegV();								// Clear registers V0-VF
	clearMem();									// Clear memory

	// Load fontset
	memcpy(memory, chip8_fontset, FONTSET_SZ);
}

void Chip8Engine::loadProgram(std::string path) {
	// Load whole program into memory, starting at address 0x200.
	// Open file.
	std::ifstream file(path, std::ios::in | std::ios::binary);

	// Get length of file.
	file.seekg(0, std::ios::end);
	size_t length = (size_t) file.tellg();
	file.seekg(0, std::ios::beg);

	// Read file into memory at address 0x200.
	file.read((char *)(memory + 0x200), length);
	file.close();
}

void Chip8Engine::emulateCycle() {
	// Fetch Opcode
	opcode = memory[cpu.pc] << 8 | memory[cpu.pc + 1]; // We have 8-bit memory, but an opcode is 16-bits long. Need to construct opcode from 2 successive memory locations.
	
    // Decode Opcode
	// Initially work out what type of opcode it is by AND with 0xF000 and branch from that (looks at MSB)
	switch (opcode & 0xF000) {
	case 0x0000:
		handleOpcodeMSN_0();
		break;
	case 0x1000:
		handleOpcodeMSN_1();
		break;
	case 0x2000:
		handleOpcodeMSN_2();
		break;
	case 0x3000:
		handleOpcodeMSN_3();
		break;
	case 0x4000:
		handleOpcodeMSN_4();
		break;
	case 0x5000:
		handleOpcodeMSN_5();
		break;
	case 0x6000:
		handleOpcodeMSN_6();
		break;
	case 0x7000:
		handleOpcodeMSN_7();
		break;
	case 0x8000:
		handleOpcodeMSN_8();
		break;
	case 0x9000:
		handleOpcodeMSN_9();
		break;
	case 0xA000:
		handleOpcodeMSN_A();
		break;
	case 0xB000:
		handleOpcodeMSN_B();
		break;
	case 0xC000:
		handleOpcodeMSN_C();
		break;
	case 0xD000:
		handleOpcodeMSN_D();
		break;
	case 0xE000:
		handleOpcodeMSN_E();
		break;
	case 0xF000:
		handleOpcodeMSN_F();
		break;
	default:
		// Unknown opcode encountered
		std::cout << "Unknown Opcode detected!";
		break;
	}

	// Update timers
	timers->updateTimers();
}

void Chip8Engine::clearGFXMem() {
	memset(gfxmem, 0, GFX_MEMORY_SZ);
}

void Chip8Engine::clearMem() {
	memset(memory, 0, MEMORY_SZ);
}

void Chip8Engine::clearRegV() {
	for (int i = 0; i < NUM_V_REG; i++) {
		cpu.V[i] = 0x0;
	}
}

void Chip8Engine::setVFRegister(uint8_t num)
{
	// Sets the VF register to the number parsed
	cpu.V[0xF] = num;
}

bool Chip8Engine::checkDrawFlag() {
	return drawflag;
}

void Chip8Engine::setDrawFlag(bool isdraw)
{
	drawflag = isdraw;
}

void Chip8Engine::setKeyState(uint8_t keyindex, KEY_STATE state)
{
	key->setKeyState(keyindex, state);
}

void Chip8Engine::DEBUG_printCPUState()
{
	using namespace std;
	// Opcode
	cout << "Opcode: " << endl;
	printf("Opcode: 0x%.4x", opcode);
	cout << endl << endl;

	// PC register
	cout << "PC Register: " << endl;
	printf("PC: 0x%.4x", cpu.pc);
	cout << endl << endl;

	// I Register
	cout << "I Register: " << endl;
	printf("I: 0x%.4x", cpu.I);
	cout << endl << endl;

	// V registers
	cout << "V Registers: " << endl;
	for (int i = 0; i < NUM_V_REG; i++) {
		printf("V[%x]: 0x%.2x, ", i, cpu.V[i]);
	}
	cout << endl << "--------------------------" << endl << endl;
	
}

void Chip8Engine::DEBUG_renderGFXText()
{
	printf("--- START RENDER ---\n\n");
	// Draw
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{
			if (gfxmem[(y * 64) + x] == 0)
				printf("O");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
	printf("--- END RENDER ---\n\n");
}

void Chip8Engine::DEBUG_printSoundTimer()
{
	if (timers->getSoundTimer() > 0) printf("\n\n*****BEEP!*****\n\n");
}

void Chip8Engine::DEBUG_printKeyState()
{
	printf("Key States: \n");
	for (int i = 0; i < 0x10; i++) {
		printf("Key[%x]: %u, ", i, key->getKeyState(i));
	}
	printf("\n\n");
}
