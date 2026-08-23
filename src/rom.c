#include "rom.h"
#include "error_helpers.h"

#include <stdio.h>

// todo better error messages
int readROMIntoMemory(chip8* emu, const char* filename){
    FILE* file = fopen(filename, "rb");

    if(file == NULL){
        fprintf(stderr, "open file error\n");
        return 1;
    }

    // move pointer to end
    int failed = fseek(file, 0, SEEK_END);
    printf("fseek result: %d\n", failed);
    if(failed != 0){
        fprintf(stderr, "fseek to end error\n");
        return 1;
    }

    // Get position measured in bytes since start of file
    long file_size = ftell(file);
    if(file_size == -1L){
        fprintf(stderr, "ftell file size error\n");
        return 1;
    }
    printf("file_size: %ld\n", file_size);
    printf("filename: %s\n", filename);
    if(file_size > 3584){
       fprintf(stderr, "read data too big %ld\n", file_size);
        return 1;
    }

    // move to begining
    failed = fseek(file, 0, SEEK_SET);
     if(failed != 0){
        fprintf(stderr, "fseek to begining again failed\n");
        return 1;
    }

    int read = fread(&emu->cpu->memory[0x200], 1, (size_t)file_size, file);
    if(read != file_size){
        return 1;
    }

    return 0;
}

int loadROM(chip8* emu, const char* filename){
    int errored = readROMIntoMemory(emu, filename);
    if(errored != 0){
        // todo change later to not use logCPUErrorAndExit
        logCPUErrorAndExit("Error reading ROM", 0x00);
        return 1;
    }

    emu->cpu->program_counter = 0x200;

    emu->cpu->CPU_state = CPU_WAIT_FOR_INPUT;

    return 0;
}
