// Super8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdint>

//#include   // OpenGL graphics and input
#include "Chip8Engine.h"

Chip8Engine * mChip8;

int main(int argc, char **argv) {
	// Set up render system and register input callbacks
	//setupGraphics();
	//setupInput();

	mChip8 = new Chip8Engine();

	// Initialize the Chip8 system and load the game into the memory  
	mChip8->initialise();
	mChip8->loadProgram("..\\chip8roms\\BRIX");

	uint64_t count = 0;
	uint8_t randstate = 0;
	for (int i = 0; i < 0x10; i++) {
		randstate = rand() % 0x2;
		mChip8->setKeyState(i, (KEY_STATE)randstate);
	}

	// Emulation loop
	for (;;)
	{
		// Emulate one cycle
		mChip8->emulateCycle();

		// Store key press state (Press and Release). DEBUG
		for (int i = 0; i < 0x10; i++) {
			randstate = rand() % 0x2;
			mChip8->setKeyState(i, (KEY_STATE)randstate);
		}
		//mChip8->DEBUG_printKeyState();
		
		if (mChip8->checkDrawFlag() ) {
			mChip8->DEBUG_printCPUState();
			mChip8->DEBUG_printSoundTimer();
			mChip8->DEBUG_renderGFXText();
			mChip8->setDrawFlag(false);
		}
		count++;
		

		// If the draw flag is set, update the screen
		if (mChip8->checkDrawFlag()) {
			//drawGraphics();
			mChip8->setDrawFlag(false);
		}
	}

	delete mChip8;

	return 0;
}