#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include "consts.h"
#include "opcodes.h"
#include "graphics.h"
#include "cpu.h"
#include "utils.h"
#include "rom.h"

/*
    Keypad       Keyboard
    +-+-+-+-+    +-+-+-+-+
    |1|2|3|C|    |1|2|3|4|
    +-+-+-+-+    +-+-+-+-+
    |4|5|6|D|    |Q|W|E|R|
    +-+-+-+-+ => +-+-+-+-+
    |7|8|9|E|    |A|S|D|F|
    +-+-+-+-+    +-+-+-+-+
    |A|0|B|F|    |Z|X|C|V|
    +-+-+-+-+    +-+-+-+-+
*/


int main(int argc, char* argv[]){

    // todo: figure out why it cannot be under 6 
    // this might break it  chip8->inputs->video[matrix_coords_to_array_coords(i, j, WINDOW_WIDTH)] = (i + j) % 2;
    int mag = 12;

    // seed rand
    srand(time(NULL));

    // Init SDL3
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Chip-8", WINDOW_WIDTH * mag, WINDOW_HEIGHT * mag, 0);
    SDL_StartTextInput(window);
    if(window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    bool done = false;

    SDL_FRect* rects = malloc(WINDOW_DIMENSIONS * sizeof(SDL_FRect) * 2);

    SDL_Event event;

    chip8* chip8 = createChip8();

    assert(chip8 != NULL);
    assert(chip8->cpu != NULL);
    assert(chip8->inputs != NULL);

    printf("video[0] = %d\n", chip8->inputs->video[0]);
    printf("video[2047] = %d\n", chip8->inputs->video[2047]);

    uint64_t last_time = SDL_GetPerformanceCounter();
    double cpu_accumulator = 0.0;
    double timer_accumulator = 0.0;

    double frequency = (double)SDL_GetPerformanceFrequency();

    while (!done) {


        uint64_t current_time = SDL_GetPerformanceCounter();

        double elapsed = (double)(current_time - last_time) / frequency;

        last_time = current_time;

        if(elapsed > 0.1){
            elapsed = 0.1;
        }

        cpu_accumulator += elapsed;
        timer_accumulator += elapsed;

        // if(chip8->cpu->video_should_clear){
        //     memset(chip8->inputs->video, 0, sizeof chip8->inputs->video);
        //     chip8->cpu->video_should_clear = false;
        // }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }

                switch (event.type) {
                    case SDL_EVENT_KEY_DOWN: {
                        int key = -1;

                        switch (event.key.scancode) {
                            case SDL_SCANCODE_X: key = 0x0; break;
                            case SDL_SCANCODE_1: key = 0x1; break;
                            case SDL_SCANCODE_2: key = 0x2; break;
                            case SDL_SCANCODE_3: key = 0x3; break;

                            case SDL_SCANCODE_Q: key = 0x4; break;
                            case SDL_SCANCODE_W: key = 0x5; break;
                            case SDL_SCANCODE_E: key = 0x6; break;
                            case SDL_SCANCODE_R: key = 0xD; break;

                            case SDL_SCANCODE_A: key = 0x7; break;
                            case SDL_SCANCODE_S: key = 0x8; break;
                            case SDL_SCANCODE_D: key = 0x9; break;
                            case SDL_SCANCODE_F: key = 0xE; break;

                            case SDL_SCANCODE_Z: key = 0xA; break;
                            case SDL_SCANCODE_C: key = 0xB; break;
                            case SDL_SCANCODE_4: key = 0xC; break;
                            case SDL_SCANCODE_V: key = 0xF; break;

                            case SDL_SCANCODE_M:
                                loadROM(chip8, argv[1]);
                                break;
                        }

                        if (key != -1) {
                            chip8->inputs->key_state[key] = true;

                            // This is an event, not a held state.
                            chip8->cpu->key_pressed = true;
                            chip8->cpu->last_key_pressed = key;
                        }

                        break;
                    }
                    case SDL_EVENT_KEY_UP:{
                        int key = -1;

                        switch (event.key.scancode) {
                            case SDL_SCANCODE_X: key = 0x0; break;
                            case SDL_SCANCODE_1: key = 0x1; break;
                            case SDL_SCANCODE_2: key = 0x2; break;
                            case SDL_SCANCODE_3: key = 0x3; break;

                            case SDL_SCANCODE_Q: key = 0x4; break;
                            case SDL_SCANCODE_W: key = 0x5; break;
                            case SDL_SCANCODE_E: key = 0x6; break;
                            case SDL_SCANCODE_R: key = 0xD; break;

                            case SDL_SCANCODE_A: key = 0x7; break;
                            case SDL_SCANCODE_S: key = 0x8; break;
                            case SDL_SCANCODE_D: key = 0x9; break;
                            case SDL_SCANCODE_F: key = 0xE; break;

                            case SDL_SCANCODE_Z: key = 0xA; break;
                            case SDL_SCANCODE_C: key = 0xB; break;
                            case SDL_SCANCODE_4: key = 0xC; break;
                            case SDL_SCANCODE_V: key = 0xF; break;
                        }

                        if (key != -1) {
                            chip8->inputs->key_state[key] = false;
                        }

                        break;
                    }
                }
            }

            double cpu_period = 1.0/CPU_HZ;

            while(cpu_accumulator >= cpu_period){

             
    
                cycle(chip8);
                cpu_accumulator -= cpu_period;

                while (timer_accumulator >= (1.0 / 60.0)) {
                    if (chip8->cpu->delay_timer > 0)
                        chip8->cpu->delay_timer--;

                    if (chip8->cpu->sound_timer > 0)
                        chip8->cpu->sound_timer--;

                    timer_accumulator -= 1.0 / 60.0;
                }
            }

          

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            int num_to_draw = drawRects(chip8->inputs->video, rects, mag);

            SDL_SetRenderDrawColor(renderer, 186, 167, 136, 255);
            SDL_RenderFillRects(renderer, rects, num_to_draw);

            SDL_RenderPresent(renderer);

    }

    return 0;
}