#include "test_cpu_helpers.h"
#include "test_helpers.h"
#include <string.h>

void testListStackContents(){
    typedef struct testCase {
        char* test_name;
        CPU CPU_state;
        char* expected;
    } testCase;

    struct testCase* testCases = (struct testCase[]){
        {
            .test_name = "stack with only one value 0x200",
            .CPU_state = {
                .stack_pointer = 0,
                .stack = {0x200}
            },
            .expected = "0200 ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" 
        },
        {
            .test_name = "empty stack",
            .CPU_state = {
                .stack_pointer = -1,
            },
            .expected = "---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" 
        },
        {
            .test_name = "full stack",
            .CPU_state = {
                .stack_pointer = 15,
                .stack = {0x010, 0x0020, 0x3000, 0x0400, 0xFFFF, 0xA0E0, 0x0144, 0xBEEF, 0xDEAD, 0xBEEF, 0x1111, 0x0087, 0x4444, 0x2211, 0x1156, 0x1010 }
            },
            .expected = "0010 0020 3000 0400 FFFF A0E0 0144 BEEF DEAD BEEF 1111 0087 4444 2211 1156 1010" 
        },
        {
            .test_name = "Stack with data past stack pointer",
            .CPU_state = {
                .stack_pointer = 5,
                .stack = {0x0010, 20, 30, 40, 255, 125, 144, 10, 21, 22, 48, 87, 4, 11, 56, 10 }
            },
            .expected = "0010 0014 001E 0028 00FF 007D ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" 
        },
    };

    for(int i = 0; i < 4; i++){
        testCase tc = testCases[i];
        CPU cpu = tc.CPU_state;
        char* got = listStackContents(&cpu);
        
        LOG_TEST(tc.test_name);
        ASSERT_STRINGS_EQUAL(tc.expected, got);
    }

}

void testCreateCPU(){

}

void runCPUHelpers(){
    testListStackContents();
}