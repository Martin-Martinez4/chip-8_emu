#include "cpu.h"
#include "digit_sprites.h"

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

    char buffer[32];
    size_t len = 0;

    for(int i = cpu->stack_pointer; i >= 0; i--){
        buffer[len++] = cpu->stack[i];

        if(i < 15 ){
            buffer[len++] = ' ';
        }

        
    }

    for(int i = cpu->stack_pointer+1; i < 15; i++ ){
        buffer[len++] = '-';
        buffer[len++] = ' ';
    }

    return buffer;
}

