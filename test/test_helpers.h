#ifndef TESTS_chip8_emu_H_HELPERS
#define TESTS_chip8_emu_H_HELPERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LOG_TEST(name) \
    printf("[TEST] %s\n", name)

#define ASSERT_TRUE(condition) do{ \
    if(condition){ \
        printf("[PASS] %s:%d: %s\n", __FILE__, __LINE__, #condition); \
    } else {\
        fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #condition); \
    } \
}while(0)

#define ASSERT_STRINGS_EQUAL(expected, actual) do { \
    if(strcmp(expected, actual) == 0) {   \
         printf("[PASS] %s:%d \n", __FILE__, __LINE__);    \
    } else {\
         fprintf(stderr, \
            "[FAIL] %s:%d: strings mismatch\n" \
            "  expected: \"%s\"\n" \
            "  actual:   \"%s\"\n", \
            __FILE__, __LINE__, (expected), (actual)); \
    } \
}while(0)

#define ASSERT_CPU_STATE_EQUAL(cpu1, cpu2) do{ \
    if(cpu1->index_register != cpu2->index_register) fprintf(stderr, "\t[FAIL]  %s:%d: \n\tindex_register\n" \
        "\t%s: %d\n"    \
        "\t%s: %d\n",   \
        __FILE__, __LINE__, #cpu1, cpu1->index_register, #cpu2, cpu2->index_register);    \
    \
    if(cpu1->program_counter != cpu2->program_counter) fprintf(stderr, "\t[FAIL]  %s:%d: \n\tprogram_counter\n" \
        "\t%s: %d\n"    \
        "\t%s: %d\n",   \
        __FILE__, __LINE__, #cpu1, cpu1->program_counter, #cpu2, cpu2->program_counter);    \
    \
    if(cpu1->stack_pointer != cpu2->stack_pointer) fprintf(stderr, "\t[FAIL]  %s:%d: \n\tstack_pointer\n" \
    "\t%s: %d\n"    \
    "\t%s: %d\n",   \
    __FILE__, __LINE__, #cpu1, cpu1->stack_pointer, #cpu2, cpu2->stack_pointer);    \
    \
    if(cpu1->delay_timer != cpu2->delay_timer) fprintf(stderr, "\t[FAIL]  %s:%d: \n\tdelay_timer\n" \
    "\t%s: %d\n"    \
    "\t%s: %d\n",   \
    __FILE__, __LINE__, #cpu1, cpu1->delay_timer, #cpu2, cpu2->delay_timer);    \
    \
    if(cpu1->sound_timer != cpu2->sound_timer) fprintf(stderr, "\t[FAIL]  %s:%d: \n\tsound_timer\n" \
    "\t%s: %d\n"    \
    "\t%s: %d\n",   \
    __FILE__, __LINE__, #cpu1, cpu1->sound_timer, #cpu2, cpu2->sound_timer)  ;  \
    \
    if(cpu1->opcode != cpu2->opcode)fprintf(stderr, "\t[FAIL]  %s:%d: \n\topcode\n" \
    "\t%s: %d\n"    \
    "\t%s: %d\n",   \
    __FILE__, __LINE__, #cpu1, cpu1->opcode, #cpu2, cpu2->opcode);    \
    \
    for(int i = 0; i < 16; i++){    \
        if(cpu1->registers[i] != cpu2->registers[i]) fprintf(stderr, "\t[FAIL]  %s:%d: \n\tregisters %d\n" \
        "\t%s: %d\n"    \
        "\t%s: %d\n",   \
        __FILE__, __LINE__, i, #cpu1, cpu1->registers[i], #cpu2, cpu2->registers[i]);    \
    }   \
    \
    char* cpu1_stacklist = listStackContents(cpu1); \
    char* cpu2_stacklist = listStackContents(cpu2); \
    if(strcmp(cpu1_stacklist, cpu2_stacklist) != 0){    \
       fprintf(stderr, "\t[FAIL]  %s:%d: \n\tstack\n" \
        "\t%s: %s\n"    \
        "\t%s: %s\n",   \
        __FILE__, __LINE__, #cpu1, cpu1_stacklist, #cpu2, cpu2_stacklist);    \
    }   \
    \
    int mem_diff_count = 0; \
    for(int i = 0; i < 4096; i++){  \
        if(mem_diff_count >= 100){   \
             fprintf(stderr, "memory diffs truncated...");   \
        }   \
        if(cpu1->memory[i] != cpu2->memory[i]) {    \
            fprintf(stderr, "\t[FAIL]  %s:%d: \n\tmemory\n" \
            "\t%s: %d\n"    \
            "\t%s: %d\n",   \
            __FILE__, __LINE__, #cpu1, cpu1->memory[i], #cpu2, cpu2->memory[i]); \
            mem_diff_count++;    \
        }    \
    }   \
} while(0) 


#endif