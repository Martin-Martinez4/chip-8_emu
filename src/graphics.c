#include "graphics.h"
#include "utils.h"

int drawRects(int* graphics_data, SDL_FRect* rects, int magnification){

    int rects_created = 0;

    for(size_t row = 0; row < WINDOW_HEIGHT; row++){
        int offsetY = magnification * row;
        for(size_t col = 0; col < WINDOW_WIDTH; col++){
            int offsetX = magnification * col;
            
           
            if(graphics_data[matrix_coords_to_array_coords(row, col, WINDOW_WIDTH)]){
                rects[rects_created++] = (SDL_FRect){.x = offsetX, .y = offsetY, .w = magnification, .h = magnification};
            }
        }
    }

    return rects_created;
}