#ifndef chip8_emu_H_GRAPHICS
#define chip8_emu_H_GRAPHICS

#include <SDL3/SDL_rect.h>
#include "consts.h"

int drawRects(int* graphics_data, SDL_FRect* rects, int magnification);

#endif
