#ifndef TESTS_chip8_emu_H_HELPERS
#define TESTS_chip8_emu_H_HELPERS

#include <stdio.h>


#define LOG_TEST(name) \
    printf("[TEST] %s\n", name)

#define ASSERT_TRUE(condition) do{ \
    if(condition){ \
        printf("[PASS] %s:%d: %s\n", __FILE__, __LINE__, #condition); \
    } else {\
        fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #condition) \
    } \
}while(0)


#endif