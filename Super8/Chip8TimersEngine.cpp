#include "stdafx.h"
#include "Chip8TimersEngine.h"


Chip8TimersEngine::Chip8TimersEngine(Chip8Engine * _chip8)
{
	chip8 = _chip8;
}


Chip8TimersEngine::~Chip8TimersEngine()
{
	chip8 = NULL;
}

void Chip8TimersEngine::updateTimers()
{
	// Update timers
	if (delay_timer > 0) delay_timer--;

	if (sound_timer > 0) {
		// if (sound_timer == 1) (std::cout << "BEEP!\n"); // TODO: implement beep
		sound_timer--;
	}
}

void Chip8TimersEngine::setSoundTimer(uint8_t num)
{
	// Set sound timer to count from
	sound_timer = num;
}

void Chip8TimersEngine::setDelayTimer(uint8_t num)
{
	// Set delay timer to count from
	delay_timer = num;
}

uint8_t Chip8TimersEngine::getSoundTimer()
{
	return sound_timer;
}

uint8_t Chip8TimersEngine::getDelayTimer()
{
	return delay_timer;
}
