#include <stdio.h>
#include <stdlib.h>
#include "opcodes.h"
#include "error_helpers.h"


void noOp(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    return;
}

// todo: Figure out error stuff
void ret(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    if(cpu->stack_pointer <= -1){
        logCPUErrorAndExit("Stack Underflow instruction", instruction);
        return;
    }

    cpu->program_counter = cpu->stack[cpu->stack_pointer];
    cpu->stack_pointer -= 1;
} 

void cls(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;

    memset(emu->inputs->video, 0, sizeof emu->inputs->video);

    
} 

// shift right 12 to get first number of opcode
// should change the name later to be more descriptive
void handle0(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    if(instruction > 0x00EEu) {noOp(emu, instruction, 0, 0); return;}
    if((0x00F0 & instruction) != 0x00E0){noOp(emu, instruction, 0, 0); return;}

    uint16_t pivot = 0x000F & instruction;

    switch(pivot) {
        case 0:
            cls(emu, instruction);
            break;
        case 0x000E:
            ret(emu, instruction);
            break;
        default:
            noOp(emu, instruction, 0, 0);
    }
}
// 1nnn
void jPAddr(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    // 0x0FFFu & instruction gets last 12 bits
    uint16_t value = 0x0FFFu & instruction;
    cpu->program_counter = value;
}
// 2nnn
void callAddr(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    if(cpu->stack_pointer > 15){
        logCPUErrorAndExit("Stack Overflow instruction", instruction);
        return;
    }

    cpu->stack_pointer++;
    cpu->stack[cpu->stack_pointer] = cpu->program_counter;
    uint16_t value = 0x0FFFu & instruction;
    cpu->program_counter = value;
}
// 3xkk
// increasing program counter by 2 effectively skips one instruction
void skipIfRegisterEqualsValue(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t value = 0x00FFu & instruction;
    uint16_t register_number = (0x0F00u & instruction) >> 8;
    if(cpu->registers[register_number] == value){cpu->program_counter += 2;}
}
// 4xkk
void skipIfRegisterNotEqualsValue(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t value = 0x00FFu & instruction;
    uint16_t register_number = (0x0F00u & instruction) >> 8;
    if(cpu->registers[register_number] != value){cpu->program_counter += 2;}
}
// 5xy0
void skipIfRegisterEqualsRegister(chip8 *emu, uint16_t instruction){
    if((0x000F & instruction) != 0) return;
    CPU *cpu = emu->cpu;

    uint16_t register_x = (0x0F00u & instruction) >> 8;
    uint16_t register_y = (0x00F0u & instruction) >> 4;

    if(cpu->registers[register_x] == cpu->registers[register_y]) cpu->program_counter += 2;
}
// 6xkk
void loadValueIntoRegister(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t register_number = (0x0F00 & instruction) >> 8;
    uint16_t value = 0x00FFu & instruction;

    cpu->registers[register_number] = value;
}
// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
void addValueToRegister(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t register_number = (0x0F00 & instruction) >> 8;
    uint16_t value = 0x00FFu & instruction;
    cpu->registers[register_number] += value;
}


// 8xy0
// set vx = vy
void loadRegisterValueFromRegister(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    cpu->registers[x_index] = cpu->registers[y_index];
}

// 8xy1
void orRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    cpu->registers[x_index] = cpu->registers[x_index] | cpu->registers[y_index];
}

// 8xy2
void andRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    cpu->registers[x_index] = cpu->registers[x_index] & cpu->registers[y_index];
}

// 8xy3
void xorRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    
    CPU *cpu = emu->cpu;
    cpu->registers[x_index] = cpu->registers[x_index] ^ cpu->registers[y_index];
}

// 8xy4
// If result greater than 8 bits (255) Vf is set to 1 otherwise 0
// store lower 8 bits of answer in Vx
void carryAddRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    
    uint8_t x = cpu->registers[x_index];
    uint8_t y = cpu->registers[y_index];

    uint16_t result = x + y;

    uint8_t vf = result > 0xFF;
    uint8_t new_x = result & 0xFF;

    cpu->registers[x_index] = new_x;
    cpu->registers[0xF] = vf;
}

//8xy5
// Vx - Vy; Vx > Vy then Vf is 1 (1 = not borrow)
// store answer in Vx
void borrowSubRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    uint8_t x =  cpu->registers[x_index];
    uint8_t y = cpu->registers[y_index];

    uint8_t vf = x > y;
    uint8_t new_x = x - y;


    cpu->registers[x_index] = new_x;
    cpu->registers[0xF] = vf;
}

// 8xy6
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. 
// Then Vx is divided by 2.
void shiftRight(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;

    uint8_t old_x = cpu->registers[x_index];
    uint8_t new_vf = old_x & 1;
    uint8_t new_x = old_x >> 1;

    cpu->registers[x_index] = new_x;
    cpu->registers[0xF] = new_vf;
}

//8xy7
// Vy - Vx; Vy > Vx then Vf is 1
// store answer in Vx
void reverseBorrowSubRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    uint16_t x =  cpu->registers[x_index];
    uint16_t y = cpu->registers[y_index];

    uint8_t vf = y >= x;
    uint8_t new_x = y - x;

    cpu->registers[x_index] = new_x;
    cpu->registers[0xF] = vf;

}

// 8xyE
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. 
// Then Vx is multiplied by 2.
void shiftLeft(chip8 *emu, uint16_t instruction, uint16_t x_index, uint16_t y_index){
    CPU *cpu = emu->cpu;
    uint16_t x =  cpu->registers[x_index];
    uint16_t msb = (x & 0x80) >> 7;

    uint8_t vf = msb;
    uint8_t new_x = (x * 2) & 0xFF;

    cpu->registers[x_index] = new_x;
    cpu->registers[0xF] = vf;
}

void handle8(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t lsb = (0x000F & instruction);
    uint16_t x_index = (0x0F00 & instruction) >> 8;
    uint16_t y_index = (0x00F0 & instruction) >> 4;

    _8xy_handlers[lsb](emu, instruction, x_index, y_index);

}

// 9xy0
void skipIfRegisterNotEqualsRegister(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    if((0x000F & instruction) != 0) return;
    uint16_t register_x = (0x0F00u & instruction) >> 8;
    uint16_t register_y = (0x00F0u & instruction) >> 4;

    if(cpu->registers[register_x] != cpu->registers[register_y]) cpu->program_counter += 2;
}

// Annn - LD I, addr
// Set I = nnn.
void LoadRegisterI(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t value = 0x0FFFu & instruction;
    cpu->index_register = value;
}

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
void jPToOffset(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t value = 0x0FFFu & instruction;
    uint16_t v0_value = cpu->registers[0];

    cpu->program_counter = value + v0_value;


}
// Cxkk - RND Vx, byte
// rand int from 0 to 255 AND it with kk store in register Vx
void storeRandInRegister(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    int random = rand() % 0xFF;
    uint16_t value = instruction & 0xFF;
    uint16_t register_x = (instruction >> 8) & 0xF;

    cpu->registers[register_x] = random & value;
}

// Dxyn
// Display n-byte sprite starting at memory location I at (Vx, Vy)
// each byte will be a new row
// Sprites xORed at existing screen 
// If this causes sprites to be earsed Vf is set to 1, else 0
// sprites wrap on overflow
void readLoadSpriteBytes(chip8 *emu, uint16_t instruction){
    uint16_t x_index = (0x0F00u & instruction) >> 8;
    uint16_t y_index = (0x00F0u & instruction) >> 4;
    uint16_t bytes = (0x000F & instruction);
    CPU *cpu = emu->cpu;
    
    cpu->registers[0xF] = 0;

    // for loop for each sprite
    for(size_t s = 0; s < bytes; s++){
        // Get sprite it is 8 bits
        uint8_t sprite_byte = cpu->memory[cpu->index_register + s];
        int y = (cpu->registers[y_index] + s) % WINDOW_HEIGHT;    
        // for loop 8 bits and xOR
        for(size_t b = 0; b < 8; b++){
            uint8_t new_px = (sprite_byte >> (7 - b)) & 1;

            int x = (cpu->registers[x_index] + b) % WINDOW_WIDTH;  

            int original_pixel_state = emu->inputs->video[matrix_coords_to_array_coords(y, x, WINDOW_WIDTH)];

            emu->inputs->video[matrix_coords_to_array_coords(y, x, WINDOW_WIDTH)] = original_pixel_state ^ new_px;

            if(new_px == 1 && original_pixel_state == 1){
                cpu->registers[0xF] = 1;
            }

        }

    }

}

void skipIfKeyPressed(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    uint8_t key = cpu->registers[x_index];


    if(emu->inputs->key_state[key]){
        cpu->program_counter += 2;
    }
} 
void skipIfKeyNotPressed(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    uint8_t key = cpu->registers[x_index];

    if(!emu->inputs->key_state[key]){
        cpu->program_counter += 2;
    }
} 

void handleE(chip8 *emu, uint16_t instruction){
    CPU *cpu = emu->cpu;
    uint16_t x_index = (0x0F00 & instruction) >> 8;
    uint16_t pivot = 0x00FF & instruction;
    
    switch (pivot)
    {
    case 0x009E:
        skipIfKeyPressed(emu, instruction, x_index);
        break;
    
    case 0X00A1:
    skipIfKeyNotPressed(emu, instruction, x_index);
        break;
    }
}

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
void loadDelayTimerValue(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    cpu->registers[x_index] = cpu->delay_timer;
}

// Fx0A LD Vx, K
// Wait for a key press, store the value of the key in Vx.  
// Everything stops until a key is pressed
void waitForKeyPress(chip8 *emu, uint16_t instruction, uint16_t x_index){
    // most the storing and such happens outside of this function
    // maybe hsould change later
    CPU *cpu = emu->cpu;
    cpu->CPU_state = CPU_WAIT_FOR_INPUT;
    cpu->wait_register = x_index;

}

// Fx15 - LD DT, Vx
// delay timer = value in dx
void setDelayTimer(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    cpu->delay_timer = cpu->registers[x_index];
}

// Fx18 - LD ST, Vx
// sound_timer  = value in dx
void setSoundTimer(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    cpu->sound_timer = cpu->registers[x_index];
}

// Fx1E 
// I = Vx + I
void addRegisterToIndex(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    cpu->index_register += cpu->registers[x_index];
}

// Fx29 - LD F, Vx
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx
// hex sprite represented by 5 uint8_t numbers
void loadHexSprite(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    cpu->index_register = 0x50 + cpu->registers[x_index] * 5;
}

// Fx33
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
// always 3 places
void storeBCDInI(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    uint16_t value = cpu->registers[x_index];
    uint16_t index = cpu->index_register;
    cpu->memory[cpu->index_register] = value / 100;
    cpu->memory[cpu->index_register + 1] = (value / 10) %10;
    cpu->memory[cpu->index_register + 2] = value % 10;

}

// Fx55
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void storeManyRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    uint16_t I = cpu->index_register;
    uint16_t stopping_point = x_index;

    for( int i = 0; i <= stopping_point; i++ ){
        cpu->memory[I + i] = cpu->registers[i];
    }
}

// Fx65
// The interpreter copies the values of registers V0 through Vx into memory, 
// starting at the address in I.
void loadManyRegisters(chip8 *emu, uint16_t instruction, uint16_t x_index){
    CPU *cpu = emu->cpu;
    uint16_t I = cpu->index_register;
    uint16_t stopping_point = x_index;

    for( int i = 0; i <= stopping_point; i++ ){
        cpu->registers[i] = cpu->memory[I + i];
    }
}

void handleF(chip8 *emu, uint16_t instruction){
    uint16_t x_index = (0x0F00 & instruction) >> 8;
    uint16_t pivot = 0x00FF & instruction;

    CPU *cpu = emu->cpu;
    switch (pivot)
    {
    case 0x0007:
        loadDelayTimerValue(emu, instruction, x_index);
        break;
    case 0x000A:
        waitForKeyPress(emu, instruction, x_index);
        break;
    case 0x0015:
        setDelayTimer(emu, instruction, x_index);
        break;
    case 0x0018:
        setSoundTimer(emu, instruction, x_index);
        break;
    case 0x001E:
        addRegisterToIndex(emu, instruction, x_index);
        break;
    case 0x0029:
        loadHexSprite(emu, instruction, x_index);
        break;
    case 0x0033:
    storeBCDInI(emu, instruction, x_index);
        break;
    case 0x0055:
        storeManyRegisters(emu, instruction, x_index);
        break;
    case 0x0065:
        loadManyRegisters(emu, instruction, x_index);
        break;
    
    default:
        break;
    }
}

// add bool key_pressed and key
void executeInstruction(chip8* emu, short instruction, bool key_pressed, int key){
    CPU *cpu = emu->cpu;
    if(cpu->CPU_state == CPU_WAIT_FOR_INPUT){
        if(key_pressed){
            cpu->registers[cpu->wait_register] = key;
            cpu->CPU_state = CPU_RUNNING;
        }

        return;
    }
    uint16_t msb = (0xF000 & instruction) >> 12;
    handlers[msb](emu, instruction);
}

void executeInstructionCycle(chip8* emu){
    CPU *cpu = emu->cpu;
    uint16_t instruction = cpu->opcode;
   
    uint16_t msb = (0xF000 & instruction) >> 12;

    handlers[msb](emu, instruction);
}

void cycle(chip8* emu){
    CPU *cpu = emu->cpu;
 
    if (cpu->CPU_state == CPU_WAIT_FOR_INPUT) {

        if (emu->cpu->key_pressed) {
            cpu->registers[cpu->wait_register] = emu->cpu->last_key_pressed;

            cpu->CPU_state = CPU_RUNNING;

            // Consume the key press
            emu->cpu->key_pressed = false;
        }

        return;
    }


    emu->cpu->opcode = 
        (emu->cpu->memory[emu->cpu->program_counter] << 8) |
        emu->cpu->memory[emu->cpu->program_counter + 1];
    emu->cpu->program_counter += 2;
    
    executeInstructionCycle(emu);
}