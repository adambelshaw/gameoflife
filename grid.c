#include "grid.h"

#define COMPRESSED_GRID_WIDTH 16 // half of GRID_WIDTH rounded up to next even number

static uint8_t _grid[COMPRESSED_GRID_WIDTH][GRID_HEIGHT];

uint8_t get_grid_value(uint8_t x, uint8_t y)
{
    uint8_t grid_value = _grid[x / 2][y];
    if (x % 2 != 0)
    {
        grid_value = grid_value >> 4; // rotate the last 4 bits to the first 4
    }
    else
    {
        grid_value = grid_value & 15; // blank out the last 4 bits
    }

    return grid_value;
}

void set_grid_value(uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t grid_value = _grid[x / 2][y];
    if (x % 2 != 0)
    {
        value = value << 4;           // rotate the first 4 bits to the last 4
        grid_value = grid_value & 15; // blank out last 4 bits of grid value
    }
    else
    {
        value = value & 15;            // blank out the last 4 bits so we don't overwrite
        grid_value = grid_value & 240; // blank out first 4 bits of grid value
    }

    grid_value = grid_value | value;
    _grid[x / 2][y] = grid_value;
}