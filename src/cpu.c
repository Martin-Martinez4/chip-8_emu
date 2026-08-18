#include "cpu.h"
#include "digit_sprites.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

CPU* createCPU(){
    CPU* cpu = malloc(sizeof(CPU));
    if(cpu == NULL){
        return NULL;
    }

    uint8_t regs[16] = {0};
    cpu->stack_pointer = -1;
    memset(cpu, 0, sizeof(CPU));

    for(size_t i = 0; i < 81; i++){
        cpu->memory[i] = digit_sprites[i];
    }

    return cpu;
}

// print out the stack content as a string
char* listStackContents(CPU* cpu){
    char buffer[128];
    size_t len = 0;

    for(int i = 0; i <= cpu->stack_pointer; i++){
        if(i < 15){

            len += snprintf(
                buffer + len,
                sizeof(buffer) - len,
                "%04X ",
                cpu->stack[i]
            );    
        }else{
            len += snprintf(
                buffer + len,
                sizeof(buffer) - len,
                "%04X",
                cpu->stack[i]
            );    
            
        }
    }

     for (int i = cpu->stack_pointer + 1; i < 16; i++) {

        if(i < 15){
            len += snprintf(
                buffer + len,
                sizeof(buffer) - len,
                "---- "
            );
        }else{
            
            len += snprintf(
                buffer + len,
                sizeof(buffer) - len,
                "----"
            );
        }
    }

    char* result = malloc(len + 1);
    if (result == NULL){
        return NULL;
    }
    memcpy(result, buffer, len + 1);
    return result;
}

// invetigate:
// Everything is started zero'd out should 
// untouched regions of memory should still equal
int cpuCpm(CPU* cpu1, CPU* cpu2){
    int diffs = 0;

    diffs += cpu1->index_register != cpu2->index_register;
    diffs += cpu1->program_counter != cpu2->program_counter;
    diffs += cpu1->stack_pointer != cpu2->stack_pointer;
    diffs += cpu1->delay_timer != cpu2->delay_timer;
    diffs += cpu1->sound_timer != cpu2->sound_timer;
    diffs += cpu1->opcode != cpu2->opcode;

    for(int i = 0; i < 16; i++){
        diffs += cpu1->registers[i] != cpu2->registers[i];
    }
    for(int i = 0; i < 16; i++){
        diffs += cpu1->stack[i] != cpu2->stack[i];
    }
    for(int i = 0; i < 4096; i++){
        diffs += cpu1->memory[i] != cpu2->memory[i];
    }

    return diffs;
}

