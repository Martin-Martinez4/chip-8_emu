#include "cpu.h"
#include "digit_sprites.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

