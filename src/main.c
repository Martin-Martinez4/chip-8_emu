#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include "consts.h"
#include "graphics.h"
#include "cpu.h"
#include "utils.h"

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

    int mag = 5;

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

    SDL_FRect* rects = (SDL_FRect*) malloc(WINDOW_DIMENSIONS * mag);

    SDL_Event event;

    chip8* chip8 = createChip8();

    while (!done) {

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            done = true;
        }

            switch (event.type) {
                case SDL_EVENT_KEY_DOWN:
                    if(event.key.scancode ==SDL_SCANCODE_0){
                        for(int i = 0; i < WINDOW_HEIGHT; i++){
                            for(int j = 0; j < WINDOW_WIDTH; j++){

                                chip8->inputs->video[matrix_coords_to_array_coords(i, j, WINDOW_WIDTH)] = (i + j) %2;
                            }
                        }
                    }
                    break;
            }
        }

        // update emu layer here

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int num_to_draw = drawRects(chip8->inputs->video, rects, mag);

        SDL_SetRenderDrawColor(renderer, 186, 167, 136, 255);
        SDL_RenderFillRects(renderer, rects, num_to_draw);

        SDL_RenderPresent(renderer);

    }

    return 0;
}