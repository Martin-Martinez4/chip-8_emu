#ifndef chip8_emu_H_OPCODES
#define chip8_emu_H_OPCODES

#include <stdint.h>
#include "cpu.h"

// All instructions are 2 bytes long and are stored most-significant-byte first. In memory, 
// the first byte of each instruction should be located at an even addresses. 
// If a program includes sprite data, it should be padded so any instructions following it 
// will be properly situated in RAM.

typedef void (*opcodeHandler)(CPU *cpu, short instruction);

opcodeHandler handlers[16]{
    handle0,
    handle1,
    handle2,
    handle3,
    handle4,
    handle5,
    handle6,
    handle7,
    handle8,
    handle9,
    handleA,
    handleB,
    handleC,
    handleD,
    handleE,
    handleF,
};

// should change the name later to be more descriptive
void handle0(CPU *cpu, short instruction){

}
void handle1(CPU *cpu, short instruction){

}
void handle2(CPU *cpu, short instruction){

}
void handle3(CPU *cpu, short instruction){

}
void handle4(CPU *cpu, short instruction){

}
void handle5(CPU *cpu, short instruction){

}
void handle6(CPU *cpu, short instruction){

}
void handle7(CPU *cpu, short instruction){

}
void handle8(CPU *cpu, short instruction){

}
void handle9(CPU *cpu, short instruction){

}
void handleA(CPU *cpu, short instruction){

}
void handleB(CPU *cpu, short instruction){

}
void handleC(CPU *cpu, short instruction){

}
void handleD(CPU *cpu, short instruction){

}
void handleE(CPU *cpu, short instruction){

}
void handleF(CPU *cpu, short instruction){

}


#endif