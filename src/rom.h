#ifndef chip8_emu_H_ROM
#define chip8_emu_H_ROM

#include "cpu.h"

int loadROM(chip8* emu, const char* filename);

#endif