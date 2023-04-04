#include <z80.h>

#define GRID_WIDTH 31
#define GRID_HEIGHT 22

#define CELL_ALIVE 0  // cell is currently alive
#define CELL_BORN 1   // cell has just been born
#define CELL_DIES 2   // cell has just died
#define CELL_ACTIVE 3 // cell neighbours an updated cell

uint8_t get_grid_value(uint8_t x, uint8_t y);
void set_grid_value(uint8_t x, uint8_t y, uint8_t value);