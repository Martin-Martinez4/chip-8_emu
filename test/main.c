#include "tests.h"
#include <stdio.h>

// todo test 9-E opcodes
// todo test f opcodes
int main(int argc, char* argv[]){
    runCPUHelpers();
    runOpcodes0To7Test();
    runOpcodes8Test();
    return 0;
}