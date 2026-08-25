#ifndef chip8_emu_H_CONSTS
#define chip8_emu_H_CONSTS

#include <SDL3/SDL.h>


#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_DIMENSIONS (WINDOW_WIDTH * WINDOW_HEIGHT)

#define CPU_HZ 700
#define TIMER_HZ 60

static const SDL_Scancode DEFAULT_BINDINGS[16] = { 
        SDL_SCANCODE_X,  // CHIP-8 0
        SDL_SCANCODE_1,  // CHIP-8 1
        SDL_SCANCODE_2,  // CHIP-8 2
        SDL_SCANCODE_3,  // CHIP-8 3
        SDL_SCANCODE_Q,  // CHIP-8 4
        SDL_SCANCODE_W,  // CHIP-8 5
        SDL_SCANCODE_E,  // CHIP-8 6
        SDL_SCANCODE_A,  // CHIP-8 7
        SDL_SCANCODE_S,  // CHIP-8 8
        SDL_SCANCODE_D,  // CHIP-8 9
        SDL_SCANCODE_Z,  // CHIP-8 A
        SDL_SCANCODE_C,  // CHIP-8 B
        SDL_SCANCODE_4,  // CHIP-8 C
        SDL_SCANCODE_R,  // CHIP-8 D
        SDL_SCANCODE_F,  // CHIP-8 E
        SDL_SCANCODE_V   // CHIP-8 F
};

#endif