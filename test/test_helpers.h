#ifndef TESTS_chip8_emu_H_HELPERS
#define TESTS_chip8_emu_H_HELPERS

#include <stdio.h>
#include <stdlib.h>


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


#endif