#ifndef chip8_emu_H_OPCODES
#define chip8_emu_H_OPCODES

#include <stdint.h>
#include "cpu.h"
#include "utils.h"


// All instructions are 2 bytes long and are stored most-significant-byte first. In memory, 
// the first byte of each instruction should be located at an even addresses. 
// If a program includes sprite data, it should be padded so any instructions following it 
// will be properly situated in RAM.

typedef void (*opcodeHandler)(chip8 *emu, uint16_t instruction);
typedef void (*opcodeHandlerWithXY)(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

void noOp(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// should change the name later to be more descriptive
void handle0(chip8 *emu, uint16_t instruction);
// 1nnn
void jPAddr(chip8 *emu, uint16_t instruction);
// 2nnn
void callAddr(chip8 *emu, uint16_t instruction);
// 3xkk
void skipIfRegisterEqualsValue(chip8 *emu, uint16_t instruction);
// 4xkk
void skipIfRegisterNotEqualsValue(chip8 *emu, uint16_t instruction);
// 5xy0
void skipIfRegisterEqualsRegister(chip8 *emu, uint16_t instruction);
// 6xkk
void loadValueIntoRegister(chip8 *emu, uint16_t instruction);
// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
void addValueToRegister(chip8 *emu, uint16_t instruction);
void handle8(chip8 *emu, uint16_t instruction);
void skipIfRegisterNotEqualsRegister(chip8 *emu, uint16_t instruction);

// Annn - LD I, addr
// Set I = nnn.
void LoadRegisterI(chip8 *emu, uint16_t instruction);

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
void jPToOffset(chip8 *emu, uint16_t instruction);
// Cxkk - RND Vx, byte
// rand int from 0 to 255 AND it with kk store in register Vx
void storeRandInRegister(chip8 *emu, uint16_t instruction);
// Dxyn
// Dispaly n-byte sprite starting at memory location I at (Vx, Vy)
// Sprites xORewd at existing screen 
// If this causes sprites to be earsed Vf is set to 1, else 0
// sprites wrap on overflow
void readLoadSpriteBytes(chip8 *emu, uint16_t instruction);
void handleE(chip8 *emu, uint16_t instruction);
void handleF(chip8 *emu, uint16_t instruction);

static opcodeHandler handlers[16] = {
    handle0,
    jPAddr,
    callAddr,
    skipIfRegisterEqualsValue,
    skipIfRegisterNotEqualsValue,
    skipIfRegisterEqualsRegister,
    loadValueIntoRegister,
    addValueToRegister,
    handle8,
    skipIfRegisterNotEqualsRegister,
    LoadRegisterI,
    jPToOffset,
    storeRandInRegister,
    readLoadSpriteBytes,
    handleE,
    handleF,
};

// 8xy0
// set vx = vy
void loadRegisterValueFromRegister(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// 8xy1
void orRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// 8xy2
void andRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// 8xy3
void xorRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// 8xy4
// If result greater than 8 bits (255) Vf is set to 1 otherwise 0
// store lower 8 bits of answer in Vx
void carryAddRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

//8xy5
// Vx - Vy; Vx > Vy then Vf is 1
// store answer in Vx
void borrowSubRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. 
// Then Vx is divided by 2.
void shiftRight(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

//8xy7
// Vy - Vx; Vy > Vx then Vf is 1
// store answer in Vx
void reverseBorrowSubRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);

// 8xyE
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. 
// Then Vx is multiplied by 2.
void shiftLeft(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index);



static opcodeHandlerWithXY _8xy_handlers[16] = {
    loadRegisterValueFromRegister,
    orRegisters,
    andRegisters,
    xorRegisters,
    carryAddRegisters,
    borrowSubRegisters,
    shiftRight,
    reverseBorrowSubRegisters,
    // 8
    noOp,
    // 9
    noOp,
    // A
    noOp,
    // B
    noOp,
    // C
    noOp,
    // D
    noOp,
    shiftLeft,
    // F
    noOp
};

void executeInstruction(chip8 *emu, short instruction, bool key_pressed, int key);
void executeInstructionCycle(chip8 *emu);
void cycle(chip8* chip8, bool key_pressed, int key);
#endif