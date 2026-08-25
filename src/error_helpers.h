#ifndef chip8_emu_H_HELPERS
#define chip8_emu_H_HELPERS

#include <stdint.h>

void logCPUErrorAndExit(char* message, uint16_t instruction);

#endif