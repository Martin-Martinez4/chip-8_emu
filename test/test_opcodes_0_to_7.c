#include "test_opcodes_0_to_7.h"

// todo:
// test void cls(CPU *cpu, uint16_t instruction)
void opcodes_0_to_7_tests(){
    typedef struct testCase {
        char* test_name;
        CPU starting_CPU_state;
        CPU wanted_CPU_state;
        short instruction; 
    } testCase;

    struct testCase* testCases = (struct testCase[]){
        // opcode 0 ret
        {
            .test_name = "ret opcode test - one thing on stack",
            .starting_CPU_state = {
                .stack_pointer = 0,
                .stack = {0x200}
            },
            .wanted_CPU_state = {
                .stack_pointer = -1,
                .stack = {0x200}
            },
            .instruction = 0x00EE 

        },
        {
            .test_name = "ret opcode test - multiple stack entries",
        },
        {
            .test_name = "ret opcode test - return from nested subroutine",
        },
        {
            .test_name = "ret opcode test - empty stack",
        },
    };
}



