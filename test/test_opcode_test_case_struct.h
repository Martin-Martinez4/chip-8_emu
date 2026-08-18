#ifndef TEST_chip8_emu_H_OPCODES_TEST_CASE_STRUCT
#define TEST_chip8_emu_H_OPCODES_TEST_CASE_STRUCT

#include "cpu.h"

typedef struct testStep{
    uint16_t instruction;
    CPU expected_CPU_state;
}testStep;

typedef struct testCase {
    char* test_name;
    CPU starting_CPU_state;
   
    testStep* steps;
    size_t step_count; 
} testCase;

typedef struct testCases{
    testCase* test_cases;
    int cases_count;
}testCases;


#endif