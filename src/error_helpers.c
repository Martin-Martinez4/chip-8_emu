#include <stdio.h>
#include <stdlib.h>
#include "error_helpers.h"

void logCPUErrorAndExit(char* message, uint16_t instruction){
    fprintf(stderr, "%s: %d\n", message, instruction);
    // exit(EXIT_FAILURE);
}



