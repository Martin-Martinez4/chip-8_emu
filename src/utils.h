#ifndef chip8_emu_H_UTILS
#define chip8_emu_H_UTILS

#define matrix_coords_to_array_coords(row, column, columns) row * columns + column \


#define array_coords_to_matrix_coords(index, columns, row, column) do{\
    row = index / columns;  \
    column = index % columns;   \
}while(0)

#endif
