#include "stdafx.h"
#include "Chip8Engine.h"

// This file provides easier opcode management rather than having it all in the main engine.cpp file.
// TODO Implement: 0x0NNN (needed ?)

void Chip8Engine::incrementPC()
{
	// Increments PC by 2 bytes (by default)
	cpu.pc = cpu.pc + 2;
}

void Chip8Engine::incrementPC(uint8_t bytes)
{
	// Increments PC by number of bytes specified.
	cpu.pc = cpu.pc + bytes;
}


void Chip8Engine::handleOpcodeMSN_0() {
	switch (opcode) {
	case 0x00E0: 
	{
		// 0x00E0: Clears the screen
		// TODO: Check if correct.
		clearGFXMem();
		// V[0xF] = 0; // Need to set VF to 0?
		setDrawFlag(true);
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x00EE:
	{
		// 0x00EE: Returns from a subroutine
		// TODO: Check if correct.
		cpu.pc = stack->getTopStack(); // Returns the previous stack address.
		incrementPC(); // Still need to advance stack by 2 as the stack returns the 'current' address.
		break;
	}
	default:
	{
		// 0x0NNN: Calls RCA 1802 program at address 0xNNN. (?)
		// TODO: Implement?. Skips instruction for now.
		incrementPC();
		break;
	}
	}
}

void Chip8Engine::handleOpcodeMSN_1() {
	// Only one subtype of opcode in this branch
	// 0x1NNN jumps to address 0xNNN (set PC)
	// TODO: check if correct
	cpu.pc = (opcode & 0x0FFF);
}

void Chip8Engine::handleOpcodeMSN_2() {
	// Only one subtype of opcode in this branch
	// 0x2NNN calls the subroutine at address 0xNNN
	stack->setTopStack(cpu.pc); // Dont want to lose the current PC, so store it on the stack
	cpu.pc = opcode & 0x0FFF; // Get the memory address within the opcode by AND
}

void Chip8Engine::handleOpcodeMSN_3() {
	// Only one subtype of opcode in this branch
	// 0x3XNN skips next instruction if VX equals NN
	// TODO: check if correct
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t num = (opcode & 0x00FF);
	if (cpu.V[vx] == num) incrementPC(4);
	else incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_4() {
	// Only one subtype of opcode in this branch
	// 0x4XNN skips next instruction if VX does not equal NN
	// TODO: check if correct
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t num = (opcode & 0x00FF);
	if (cpu.V[vx] != num) incrementPC(4);
	else incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_5() {
	// Only one subtype of opcode in this branch
	// 0x5XY0 skips next instruction if VX equals XY
	// TODO: check if correct
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	if (cpu.V[vx] == cpu.V[vy]) incrementPC(4);
	else incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_6() {
	// Only one subtype of opcode in this branch
	// 0x6XNN sets VX to NN
	// TODO: check if correct
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t num = (opcode & 0x00FF);
	cpu.V[vx] = num;
	incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_7() {
	// Only one subtype of opcode in this branch
	// 0x7XNN adds NN to Vx
	// TODO: check if correct
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t num = (opcode & 0x00FF);
	cpu.V[vx] = cpu.V[vx] + num;
	incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_8() {
	switch (opcode & 0x000F) {
	case 0x0000:
	{
		// 0x8XY0: Sets VX to the value of VY
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.V[vx] = cpu.V[vy]; // Set Vx to Vy
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0001:
	{
		// 0x8XY1: Sets VX to VX OR VY
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.V[vx] = cpu.V[vx] | cpu.V[vy]; // Set Vx to (Vx | Vy)
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0002:
	{
		// 0x8XY2: Sets VX to VX AND VY
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.V[vx] = cpu.V[vx] & cpu.V[vy]; // Set Vx to (Vx & Vy)
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0003:
	{
		// 0x8XY3: Sets VX to VX XOR VY
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.V[vx] = cpu.V[vx] ^ cpu.V[vy]; // Set Vx to (Vx ^ Vy)
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0004:
	{
		// 0x8XY4: Adds Vy to Vx, setting VF to 1 when there is a carry and 0 when theres not.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		if (cpu.V[vy] > (0xFF - cpu.V[vx])) cpu.V[0xF] = 1; // If Vy is larger than 255 - Vx, then the result will overflow and the carry flag will be set to 1.
		else cpu.V[0xF] = 0; // The result does not overflow and carry flag is set to 0.
		cpu.V[vx] = cpu.V[vx] + cpu.V[vy]; // Perform opcode
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0005:
	{
		// 0x8XY5: Vy is subtracted from Vx. VF set to 0 when theres a borrow, and 0 when there isnt.
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		if (cpu.V[vy] > cpu.V[vx]) cpu.V[0xF] = 0; // If Vy is larger than Vx, then the result will underflow and the carry flag will be set to 0.
		else cpu.V[0xF] = 1; // The result does not underflow and carry flag is set to 1. 
		cpu.V[vx] = cpu.V[vx] - cpu.V[vy]; // Perform opcode
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0006:
	{
		// 0x8XY6: Shifts Vx right by one. VF is set to the LSB of Vx before the shift.
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		cpu.V[0xF] = cpu.V[vx] & 0x01; // Set VF to LSB of Vx.
		cpu.V[vx] = cpu.V[vx] >> 1; // Perform opcode
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x0007:
	{
		// 0x8XY7: Sets Vx to Vy minus Vx. VF is set to 0 when theres a borrow, and 1 where there isnt.
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 8 to get to a single base16 digit.
		if (cpu.V[vx] > cpu.V[vy]) cpu.V[0xF] = 0; // If Vx is larger than Vy, then the result will underflow and the carry flag will be set to 0.
		else cpu.V[0xF] = 1; // The result does not underflow and carry flag is set to 1. 
		cpu.V[vx] = cpu.V[vy] - cpu.V[vx]; // Perform opcode
		incrementPC(); // Goto next opcode
		break;
	}
	case 0x000E:
	{
		// 0x8XYE: Shifts Vx left by one. VF is set to the value of the MSB of Vx before the shift.
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 16 to get to a single base16 digit.
		cpu.V[0xF] = (cpu.V[vx] & 0x80) >> 7; // Set VF to MSB of Vx. 0x80 = 0b10000000 and need to shift to the right by 7 places.
		cpu.V[vx] = cpu.V[vx] << 1; // Perform opcode
		incrementPC(); // Goto next opcode
		break;
	}
	default:
	{
		std::cout << "Unknown Opcode detected (in 0x8000)" << std::endl;
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	}
}

void Chip8Engine::handleOpcodeMSN_9() {
	switch (opcode & 0x000F) {
	case 0x0000:
	{
		// 0x9XY0: Skips next instruction if register VX does not equal register VY
		// TODO: Check if correct
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 4 to get to a single base16 digit.
		if (cpu.V[vx] != cpu.V[vy]) incrementPC(4); // Check if the two registers are not equal, and skip next instruction if true (skips next 2 bytes). 
		else incrementPC(); // Else execute next instruction.
		break;
	}
	default:
	{
		std::cout << "Unknown Opcode detected (in 0x9000)" << std::endl;
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	}
}

void Chip8Engine::handleOpcodeMSN_A() {
	// Only one subtype of opcode in this branch
	// 0xANNN: Sets I to the address NNN
	// TODO: Check if correct
	cpu.I = (opcode & 0x0FFF); // Set I to address NNN
	incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_B() {
	// Only one subtype of opcode in this branch
	// 0xBNNN: Sets PC to the address (NNN + V0)
	// TODO: Check if correct
	cpu.pc = ((opcode & 0x0FFF) + cpu.V[0x0]);
}

void Chip8Engine::handleOpcodeMSN_C() {
	// Only one subtype of opcode in this branch
	// 0xCXNN: Sets Vx to the result of 0xNN & (random number)
	// TODO: Check if correct.
	uint8_t randnum = rand() % 256; // Get random number from 0 -> 255.
	uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
	uint8_t opcodenum = opcode & 0x0FF; // Number from opcode.
	cpu.V[vx] = opcodenum & randnum; // Set Vx to number from opcode AND random number.
	incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_D() {
	// Only one subtype of opcode in this branch
	/* 0xDXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
	           Each row of 8 pixels is read as bit-coded starting from memory location I; 
	           I value doesn’t change after the execution of this instruction.
	           As described above, VF is set to 1 if any screen pixels are flipped from 
	           set to unset when the sprite is drawn, and to 0 if that doesn’t happen */
	// TODO: check if correct. 
	uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
	uint8_t vy = (opcode & 0x00F0) >> 4; // Need to bit shift by 4 to get to a single base16 digit.

	uint8_t xpixel = cpu.V[vx]; // Get x-pixel position from register Vx
	uint8_t ypixel = cpu.V[vy]; // Get y-pixel position from register Vy
	uint8_t nrows = (opcode & 0x000F); // Get num of rows to draw.
	uint32_t gfxarraypos = 0x0; // Variable used to calculate position within gfx memory array based on x and y positions.

	uint8_t newpixeldata = 0x0; // Variable used to hold the new pixel data from memory[I, I+1] etc.
	uint8_t oldpixelbit = 0x0; // Variable used to hold old pixel bit currently on screen.
	uint8_t newpixelbit = 0x0; // Variable used to hold new pixel bit, grabbed from the newpixeldata variable.

	cpu.V[0xF] = 0; // Set VF to 0 initially (from specs).

	for (int ypos = 0; ypos < nrows; ypos++) { // Loop through number of rows to display from opcode.
		newpixeldata = memory[cpu.I + ypos]; // Get the first row of pixel data.
		for (int xpos = 0; xpos < NUM_BITS_PER_BYTE; xpos++) { // Loop though the x-positions.
			gfxarraypos = ((ypixel + ypos) * GFX_XRES) + (xpixel + xpos); // Calculate the gfx memory array position
			newpixelbit = (newpixeldata & (0x80 >> xpos)) >> xpos; // Get the pixel bit value from within the 8-bit data. 
														 // 0x01 = 0b00000001 ie: mask with a single bit on each xpos loop.
			if (newpixelbit != 0) { // Set new pixel only if there is data.
				oldpixelbit = gfxmem[gfxarraypos]; // Get the previous pixel value (already in the form of 1 or 0).
				if (oldpixelbit == 1) cpu.V[0xF] = 1; // Set VF to 1 if pixel will be unset (from specs, used for collision detection).
				gfxmem[gfxarraypos] = gfxmem[gfxarraypos] ^ 0x01; // Toggle pixel using XOR.
			}
		}
	}

	setDrawFlag(true); // Set the draw flag to true.
	incrementPC(); // Update PC by 2 bytes
}

void Chip8Engine::handleOpcodeMSN_E() {
	switch (opcode & 0x00FF) {
	case 0x009E:
	{
		// 0xEX9E: Skips the next instruction if the key stored in Vx is pressed.
		// TODO: Check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		uint8_t keynum = cpu.V[vx]; // Get the key number from registry Vx.
		if (key->getKeyState(keynum)) incrementPC(4); // Skip next instruction if key is pressed.
		else incrementPC(); // Else goto next instruction.
		break;
	}
	case 0x00A1:
	{
		// 0xEXA1: Skips the next instruction if the key stored in Vx isnt pressed.
		// TODO: Check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		uint8_t keynum = cpu.V[vx]; // Get the key number from registry Vx.
		if (!key->getKeyState(keynum)) incrementPC(4); // Skip next instruction if key is not pressed.
		else incrementPC(); // Else goto next instruction.
		break;
	}
	default:
	{
		std::cout << "Unknown Opcode detected (in 0xE000)" << std::endl;
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	}
}

void Chip8Engine::handleOpcodeMSN_F() {
	switch (opcode & 0x00FF) {
	case 0x0007:
	{
		// 0xFX07: Sets Vx to the value of the delay timer.
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.V[vx] = timers->getDelayTimer(); // Get delay timer and store it in Vx.
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x000A:
	{
		// 0xFX0A: A key press is awaited, then stored in Vx.
		// TODO: Check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		for (int i = 0; i <= 0xF; i++) {
			uint8_t keystate = key->getKeyState(i); // Get the keystate from the key object.
			if (keystate) cpu.V[vx] = i; // Set Vx to the key pressed (0x0 -> 0xF).
			else return; // If no key was pressed, check again next cycle (by not incrementing PC)
		}
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x0015:
	{
		// 0xFX15: Sets the delay timer to Vx.
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		timers->setDelayTimer(cpu.V[vx]);
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x0018:
	{
		// 0xFX18: Sets the sound timer to Vx.
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		timers->setSoundTimer(cpu.V[vx]);
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x001E:
	{
		// 0xFX1E: Adds Vx to I.		
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.I = cpu.I + cpu.V[vx];
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x0029:
	{
		// 0xFX29: Sets I to the location of the sprite for the character in Vx. Chars 0-F (in hex) are represented by a 4x5 font.
		//         ie: if V[x] = 0xA, set I to location of A in font sheet. Note that sprites are 8-bits wide, while fonts are 4-bits, so
		//             the 4-bits at the end are padded (0's).
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		cpu.I = cpu.V[vx] * FONT_WIDTH; // Set I to the location of the first byte of the font needed within the font set.
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x0033:
	{
		// 0xFX33: Splits the decimal representation of Vx into 3 locations: hundreds stored in address I, tens in address I+1, and ones in I+2.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		/* DEBUG
		uint8_t vxval = cpu.V[vx];

		uint8_t hundreds = cpu.V[vx] / 100;
		uint8_t tens = (cpu.V[vx] % 100) / 10;
		uint8_t ones = (cpu.V[vx] % 100) % 10;

		uint16_t Ip0 = cpu.I;
		uint16_t Ip1 = cpu.I + 1;
		uint16_t Ip2 = cpu.I + 2;
		*/
		memory[cpu.I] = cpu.V[vx] / 100; // Hundreds go into address I
		memory[cpu.I + 1] = (cpu.V[vx] % 100) / 10; // Tens go into address I+1
		memory[cpu.I + 2] = (cpu.V[vx] % 100) % 10 /* / 1 */; // Ones go into address I+2
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x0055:
	{
		// 0xFX55: Copies all current values in registers V0 -> Vx to memory starting at address I.
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		for (int i = 0x0; i <= vx; i++) {
			memory[cpu.I + i] = cpu.V[i];
		}
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	case 0x0065:
	{
		// 0xFX65: Copies memory starting from address I to all registers V0 -> Vx.
		// TODO: check if correct.
		uint8_t vx = (opcode & 0x0F00) >> 8; // Need to bit shift by 8 to get to a single base16 digit.
		for (int i = 0x0; i <= vx; i++) {
			cpu.V[i] = memory[cpu.I + i];
		}
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	default: 
	{
		std::cout << "Unknown Opcode detected (in 0xF000)" << std::endl;
		incrementPC(); // Update PC by 2 bytes
		break;
	}
	}
}