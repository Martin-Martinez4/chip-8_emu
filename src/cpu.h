#ifndef chip8_emu_H_CPU
#define chip8_emu_H_CPU

#include <stdint.h>
#include "consts.h"


typedef enum {
    CPU_IDLE,
    CPU_RUNNING,
    CPU_WAIT_FOR_INPUT
} CpuState;
// Information From: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

// Memory structure
// 0x000-0x1FF: Originally reserved for the CHIP-8 interpreter, 
// but in our modern emulator we will just never write to or read from that 
// area. Except for…

// 0x050-0x0A0: Storage space for the 16 built-in characters (0 through F), 
// which we will need to manually put into our memory because ROMs will be looking 
// for those characters.

// 0x200-0xFFF: Instructions from the ROM will be stored starting at 0x200, 
// and anything left after the ROM’s space is free to use.

typedef struct CPU {
    // 16 8 bit registers
    // v0 to vF
    uint8_t registers[16];
    uint8_t memory[4096];

    // stores memory address 16-bit
    // points to a place in 4kb memory
    uint16_t index_register;


    // points to next instruction of course
    uint16_t program_counter;

    // 16-level stack
    uint16_t stack[16];

    // keep track of register to fill after waiting
    // Not part of actually specs
    uint16_t wait_register;

    int8_t stack_pointer;

    uint8_t delay_timer;
    uint8_t sound_timer;
   
    uint16_t opcode;

    CpuState CPU_state;

    uint8_t last_key_pressed;
    bool key_pressed; 

    bool frame_ready;


}CPU;

// screen and input may have to be global
typedef struct inputs {
    // display is 64 px wide by 32 px tall
    // originally only on or off monochrome
    int video[WINDOW_DIMENSIONS];

 
    int keypad[16];
    bool key_state[16];

}inputs;

typedef struct chip8 {
    CPU* cpu;
    inputs* inputs;
}chip8;


CPU* createCPU();
chip8* createChip8();


// print out the stack content as a string
char* listStackContents(CPU* cpu);
int cpuCpm(CPU* cpu1, CPU* cpu2);


#endif