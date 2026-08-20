#include <stdio.h>
#include <stdlib.h>
#include "opcodes.h"
#include "error_helpers.h"


void noOp(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    return;
}

// todo: Figure out error stuff
void ret(CPU *cpu, uint16_t instruction){
    if(cpu->stack_pointer <= -1){
        logCPUErrorAndExit("Stack Underflow instruction", instruction);
        return;
    }

    cpu->program_counter = cpu->stack[cpu->stack_pointer];
    cpu->stack_pointer -= 1;
} 

void cls(CPU *cpu, uint16_t instruction){

} 

// shift right 12 to get first number of opcode
// should change the name later to be more descriptive
void handle0(CPU *cpu, uint16_t instruction){
    if(instruction > 0x00EEu) {noOp(cpu, instruction, 0, 0); return;}
    if((0x00F0 & instruction) != 0x00E0){noOp(cpu, instruction, 0, 0); return;}

    uint16_t pivot = 0x000F & instruction;

    switch(pivot) {
        case 0:
            cls(cpu, instruction);
            break;
        case 0x000E:
            ret(cpu, instruction);
            break;
        default:
            noOp(cpu, instruction, 0, 0);
    }
}
// 1nnn
void jPAddr(CPU *cpu, uint16_t instruction){
    // 0x0FFFu & instruction gets last 12 bits
    uint16_t value = 0x0FFFu & instruction;
    cpu->program_counter = value;
}
// 2nnn
void callAddr(CPU *cpu, uint16_t instruction){
    if(cpu->stack_pointer > 15){
        logCPUErrorAndExit("Stack Overflow instruction", instruction);
    }

    cpu->stack_pointer++;
    cpu->stack[cpu->stack_pointer] = cpu->program_counter;
    uint16_t value = 0x0FFFu & instruction;
    cpu->program_counter = value;
}
// 3xkk
// increasing program counter by 2 effectively skips one instruction
void skipIfRegisterEqualsValue(CPU *cpu, uint16_t instruction){
    uint16_t value = 0x00FFu & instruction;
    uint16_t register_number = (0x0F00u & instruction) >> 8;
    if(cpu->registers[register_number] == value){cpu->program_counter += 2;}
}
// 4xkk
void skipIfRegisterNotEqualsValue(CPU *cpu, uint16_t instruction){
    uint16_t value = 0x00FFu & instruction;
    uint16_t register_number = (0x0F00u & instruction) >> 8;
    if(cpu->registers[register_number] != value){cpu->program_counter += 2;}
}
// 5xy0
void skipIfRegisterEqualsRegister(CPU *cpu, uint16_t instruction){

    if((0x000F & instruction) != 0) return;

    uint16_t register_x = (0x0F00u & instruction) >> 8;
    uint16_t register_y = (0x00F0u & instruction) >> 4;

    if(cpu->registers[register_x] == cpu->registers[register_y]) cpu->program_counter += 2;
}
// 6xkk
void loadValueIntoRegister(CPU *cpu, uint16_t instruction){
    uint16_t register_number = (0x0F00 & instruction) >> 8;
    uint16_t value = 0x00FFu & instruction;

    cpu->registers[register_number] = value;
}
// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
void addValueToRegister(CPU *cpu, uint16_t instruction){
    uint16_t register_number = (0x0F00 & instruction) >> 8;
    uint16_t value = 0x00FFu & instruction;
    cpu->registers[register_number] += value;
}


// 8xy0
// set vx = vy
void loadRegisterValueFromRegister(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    cpu->registers[x_index] = cpu->registers[y_index];
}

// 8xy1
void orRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    cpu->registers[x_index] = cpu->registers[x_index] | cpu->registers[y_index];
}

// 8xy2
void andRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    cpu->registers[x_index] = cpu->registers[x_index] & cpu->registers[y_index];
}

// 8xy3
void xorRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    cpu->registers[x_index] = cpu->registers[x_index] ^ cpu->registers[y_index];
}

// 8xy4
// If result greater than 8 bits (255) Vf is set to 1 otherwise 0
// store lower 8 bits of answer in Vx
void carryAddRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    uint16_t value = cpu->registers[x_index] + cpu->registers[y_index];
    if(value > 255){
        cpu->registers[0x00F] = 1;
        value = value & 0x00FF;
    }else{
        cpu->registers[0x00F] = 0;
    }

    cpu->registers[x_index] = value;
}

//8xy5
// Vx - Vy; Vx > Vy then Vf is 1 (1 = not borrow)
// store answer in Vx
void borrowSubRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    uint16_t x_value =  cpu->registers[x_index];
    uint16_t y_value = cpu->registers[y_index];

    if(x_value > y_value){
        cpu->registers[0x00F] = 1;
    }else{
        cpu->registers[0x00F] = 0;
    }

    cpu->registers[x_index] = x_value - y_value;

}

// 8xy6
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. 
// Then Vx is divided by 2.
void shiftRight(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    uint16_t value = cpu->registers[x_index];
    cpu->registers[0x000F] = value % 2;
    cpu->registers[x_index] = value / 2;
}

//8xy7
// Vy - Vx; Vy > Vx then Vf is 1
// store answer in Vx
void reverseBorrowSubRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    uint16_t x_value =  cpu->registers[x_index];
    uint16_t y_value = cpu->registers[y_index];

    if(y_value > x_value){
        cpu->registers[0x00F] = 1;
    }else{
        cpu->registers[0x00F] = 0;
    }

    cpu->registers[x_index] = y_value - x_value;

}

// 8xyE
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. 
// Then Vx is multiplied by 2.
void shiftLeft(CPU *cpu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    uint16_t x_value =  cpu->registers[x_index];
    uint16_t msb = (x_value & 0x80) >> 7;
    if(msb == 1){
         cpu->registers[0x00F] = 1;
    }else{
        cpu->registers[0x00F] = 0;
    }

    cpu->registers[x_index] = x_value * 2;
}

void handle8(CPU *cpu, uint16_t instruction){
    uint16_t lsb = (0x000F & instruction);
    uint16_t x_index = (0x0F00 & instruction) >> 8;
    uint16_t y_index = (0x00F0 & instruction) >> 4;

    _8xy_handlers[lsb](cpu, instruction, x_index, y_index);

}

// 9xy0
void skipIfRegisterNotEqualsRegister(CPU *cpu, uint16_t instruction){
    if((0x000F & instruction) != 0) return;
    uint16_t register_x = (0x0F00u & instruction) >> 8;
    uint16_t register_y = (0x00F0u & instruction) >> 4;

    if(cpu->registers[register_x] != cpu->registers[register_y]) cpu->program_counter += 2;
}

// Annn - LD I, addr
// Set I = nnn.
void LoadRegisterI(CPU *cpu, uint16_t instruction){
    uint16_t value = 0x0FFFu & instruction;
    cpu->index_register = value;
}

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
void jPToOffset(CPU *cpu, uint16_t instruction){
    uint16_t value = 0x0FFFu & instruction;
    uint16_t v0_value = cpu->registers[0];

    cpu->program_counter = value + v0_value;


}
// Cxkk - RND Vx, byte
// rand int from 0 to 255 AND it with kk store in register Vx
void storeRandInRegister(CPU *cpu, uint16_t instruction){
    int random = rand() % 256;
    uint16_t value = 0x00FF & instruction;
    uint16_t register_x = (0x0F00 & instruction) >> 8;

    cpu->registers[register_x] = value + random;
}
// Dxyn
// Display n-byte sprite starting at memory location I at (Vx, Vy)
// Sprites xORed at existing screen 
// If this causes sprites to be earsed Vf is set to 1, else 0
// sprites wrap on overflow
void readLoadSpriteBytes(CPU *cpu, uint16_t instruction){
    // todo
}



void skipIfKeyPressed(CPU *cpu, uint16_t instruction, uint16_t x_index){
    // todo
} 
void skipIfKeyNotPressed(CPU *cpu, uint16_t instruction, uint16_t x_index){
    // todo
} 

void handleE(CPU *cpu, uint16_t instruction){
    uint16_t x_index = (0x0F00 & instruction) >> 8;
    uint16_t pivot = 0x00FF & instruction;
    
    switch (pivot)
    {
    case 0x009E:
        skipIfKeyPressed(cpu, instruction, x_index);
        break;
    
    case 0X00A1:
    skipIfKeyNotPressed(cpu, instruction, x_index);
        break;
    }
}

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
void loadDelayTimerValue(CPU *cpu, uint16_t instruction, uint16_t x_index){
    cpu->registers[x_index] = cpu->delay_timer;
}

// Fx0A LD Vx, K
// Wait for a key press, store the value of the key in Vx.  
// Everything stops until a key is pressed
void waitForKeyPress(CPU *cpu, uint16_t instruction, uint16_t x_index){
    // todo
}

// Fx15 - LD DT, Vx
// delay timer = value in dx
void setDelayTimer(CPU *cpu, uint16_t instruction, uint16_t x_index){
    cpu->delay_timer = cpu->registers[x_index];
}

// Fx18 - LD ST, Vx
// sound_timer  = value in dx
void setSoundTimer(CPU *cpu, uint16_t instruction, uint16_t x_index){
    cpu->sound_timer = cpu->registers[x_index];
}

// Fx1E 
// I = Vx + I
void addRegisterToIndex(CPU *cpu, uint16_t instruction, uint16_t x_index){
    cpu->index_register += cpu->registers[x_index];
}

// Fx29 - LD F, Vx
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx
// hex sprite represented by 5 uint8_t numbers
void loadHexSprite(CPU *cpu, uint16_t instruction, uint16_t x_index){
    cpu->index_register = cpu->registers[x_index] * 5;
}

// Fx33
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
void storeBCDInI(CPU *cpu, uint16_t instruction, uint16_t x_index){
    // todo
}

// Fx55
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void storeManyRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index){
    uint16_t I = cpu->index_register;
    uint16_t stopping_point = cpu->registers[x_index];

    for( int i = 0; i <= stopping_point; i++ ){
        cpu->registers[i] = cpu->memory[I + i];
    }
}

// Fx65
// The interpreter copies the values of registers V0 through Vx into memory, 
// starting at the address in I.
void loadManyRegisters(CPU *cpu, uint16_t instruction, uint16_t x_index){
    uint16_t I = cpu->index_register;
    uint16_t stopping_point = cpu->registers[x_index];

    for( int i = 0; i <= stopping_point; i++ ){
        cpu->memory[ I + i] = cpu->registers[i];
    }
}

void handleF(CPU *cpu, uint16_t instruction){
    uint16_t x_index = (0x0F00 & instruction) >> 8;
    uint16_t pivot = 0x00FF & instruction;

    switch (pivot)
    {
    case 0x0007:
        loadDelayTimerValue(cpu, instruction, x_index);
        break;
    case 0x000A:
        waitForKeyPress(cpu, instruction, x_index);
        break;
    case 0x0015:
        setDelayTimer(cpu, instruction, x_index);
        break;
    case 0x0018:
        setSoundTimer(cpu, instruction, x_index);
        break;
    case 0x001E:
        addRegisterToIndex(cpu, instruction, x_index);
        break;
    case 0x0029:
        loadHexSprite(cpu, instruction, x_index);
        break;
    case 0x0033:
    storeBCDInI(cpu, instruction, x_index);
        break;
    case 0x0055:
        storeManyRegisters(cpu, instruction, x_index);
        break;
    case 0x0065:
        loadManyRegisters(cpu, instruction, x_index);
        break;
    
    default:
        break;
    }
}

void executeInstruction(CPU* cpu, short instruction){
    uint16_t msb = (0xF000 & instruction) >> 12;
    handlers[msb](cpu, instruction);
}