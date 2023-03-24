#include "grid.h"

#define COMPRESSED_GRID_WIDTH 16 // half of GRID_WIDTH rounded up to next even number

unsigned char _grid[COMPRESSED_GRID_WIDTH][GRID_HEIGHT];

unsigned char getGridValue(unsigned char x, unsigned char y)
{
    unsigned char gridValue = _grid[x / 2][y];
    if (x % 2 != 0) {        
        gridValue = gridValue >> 4; // rotate the last 4 bits to the first 4
    } else {        
        gridValue = gridValue & 15; // blank out the last 4 bits
    }

    return gridValue;
}

void setGridValue(unsigned char x, unsigned char y, unsigned char value)
{   
    unsigned char gridValue = _grid[x / 2][y]; 
    if (x % 2 != 0) {
        value = value << 4; // rotate the first 4 bits to the last 4        
        gridValue = gridValue & 15; // blank out last 4 bits of grid value
    } else {        
        value = value & 15; // blank out the last 4 bits so we don't overwrite        
        gridValue = gridValue & 240; // blank out first 4 bits of grid value
    }
    
    gridValue = gridValue | value;
    _grid[x / 2][y] = gridValue;
}

unsigned int getCellLocation(unsigned char x, unsigned char y)
{
    unsigned int cellLocation = 0;
    cellLocation = GRID_WIDTH * y;
    cellLocation += x + 1;
    return cellLocation;
}

unsigned char getCellXCoord(unsigned int cellLocation)
{
    return (cellLocation % GRID_WIDTH) - 1;
}

unsigned char getCellYCoord(unsigned int cellLocation)
{
    return cellLocation / GRID_WIDTH;
}