#include "grid.h"

// imported from grid.asm
extern uint8_t get_grid_value_asm(uint8_t x, uint8_t y) __z88dk_callee;
extern void set_grid_value_asm(uint8_t x, uint8_t y, uint16_t value) __z88dk_callee; // last parameter uint16_t as crashes if odd number of 8 bits

uint8_t get_grid_value(uint8_t x, uint8_t y)
{
    return get_grid_value_asm(x, y);
}

void set_grid_value(uint8_t x, uint8_t y, uint8_t value)
{
    set_grid_value_asm(x, y, value);
}