#include "tests.h"
#include <stdio.h>

int main(int argc, char* argv[]){
    runCPUHelpers();
    runOpcodes0To7Test();
    runOpcodes8Test();
    return 0;
}