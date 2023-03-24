#include "screen.h"
#include "grid.h"

#define MAX_UPDATED_CELLS 50
#define MAX_ACTIVE_CELLS 200

#define CELL_ALIVE 0 // cell is currently alive
#define CELL_BORN 1 // cell has just been born
#define CELL_DIES 2 // cell has just died
#define CELL_ACTIVE 3 // cell neighbours an updated cell

unsigned int _updatedCellCount = 0;
unsigned int _updatedCells[MAX_UPDATED_CELLS];
unsigned int _activeCellCount = 0;
unsigned int _activeCells[MAX_ACTIVE_CELLS];

unsigned char isBitSet(unsigned char ch, unsigned char i)
{
    unsigned char mask = 1 << i;
    return mask & ch;
}

unsigned char setBit(unsigned char ch, unsigned char i)
{
    unsigned char mask = 1 << i;
    return ch | mask;
}

unsigned char clearBit(unsigned char ch, unsigned char i)
{
    unsigned char mask = 1 << i;
    mask = ~mask;
    return ch & mask;
}

void drawGrid()
{
    // draw grid and update active cells for update method
    _activeCellCount = 0;
    unsigned int i = 0;
    for (i = 0; i < _updatedCellCount; i++) {

        unsigned int cellLocation = _updatedCells[i];
        unsigned char x = getCellXCoord(cellLocation);
        unsigned char y = getCellYCoord(cellLocation);
        unsigned char gridValue = getGridValue(x, y);
        if (isBitSet(gridValue, CELL_ALIVE)) {
            printBlock(x, y);
            gridValue = clearBit(gridValue, CELL_BORN);
        } else {
            clearBlock(x, y);
            gridValue = clearBit(gridValue, CELL_DIES);
            
        }
        setGridValue(x, y, gridValue);

        int x2 = 0;
        int y2 = 0;
        for (x2 = x - 1; x2 <= x + 1; x2++) {
            if (x2 >= 0 && x2 < GRID_WIDTH) {
                for (y2 = y - 1; y2 <= y + 1; y2++) {
                    unsigned char gridValue = getGridValue(x2, y2);
                    if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && !isBitSet(gridValue, CELL_ACTIVE) && _activeCellCount < MAX_ACTIVE_CELLS) {
                        _activeCells[_activeCellCount++] = getCellLocation(x2, y2);
                        gridValue = setBit(gridValue, CELL_ACTIVE);
                        setGridValue(x2, y2, gridValue);
                    }
                }
            }
        }
    }
}

unsigned char wasCellAlive(unsigned char x, unsigned char y)
{
    unsigned char gridValue = getGridValue(x, y);
    if ((isBitSet(gridValue, CELL_ALIVE) && !isBitSet(gridValue, CELL_BORN)) || isBitSet(gridValue, CELL_DIES)) {
        return 1;
    }
    return 0;
}

unsigned char updateCellState(unsigned char x, unsigned char y)
{
    unsigned char total = 0;
    int x2 = 0;
    int y2 = 0;
    for (x2 = x - 1; x2 <= x + 1; x2++) {
        if (x2 >= 0 && x2 < GRID_WIDTH) {
            for (y2 = y - 1; y2 <= y + 1; y2++) {
                if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && wasCellAlive(x2, y2)) {
                    total++;
                }
            }
        }
    }
    
    unsigned char wasAlive = wasCellAlive(x, y);
    
    unsigned char gridValue = 0;
    if (wasAlive == 1 && total > 1 && total < 4) {
        gridValue = setBit(gridValue, CELL_ALIVE);
    } else if (wasAlive == 0 && total == 3) {
        gridValue = setBit(gridValue, CELL_ALIVE);
        gridValue = setBit(gridValue, CELL_BORN);
    } else if (wasAlive == 1) {
        gridValue = setBit(gridValue, CELL_DIES);
    }
    setGridValue(x, y, gridValue);
    return gridValue;
}

void iterateGrid()
{    
    // update grid and populate updated cells for draw method
    _updatedCellCount = 0;
    unsigned int i = 0;
    for (i = 0; i < _activeCellCount; i++) {
        unsigned int cellLocation = _activeCells[i];
        unsigned char x = getCellXCoord(cellLocation);
        unsigned char y = getCellYCoord(cellLocation);
        unsigned char gridValue = updateCellState(x, y);
        if ((isBitSet(gridValue, CELL_BORN) || isBitSet(gridValue, CELL_DIES)) && _updatedCellCount < MAX_UPDATED_CELLS) {
            _updatedCells[_updatedCellCount++] = cellLocation;            
        }
    }
}

void addCell(unsigned char x, unsigned char y)
{
    if (_updatedCellCount < MAX_UPDATED_CELLS) {
        unsigned char gridValue = setBit(0, CELL_ALIVE);
        gridValue = setBit(gridValue, CELL_BORN);
        setGridValue(x, y, gridValue);
        _updatedCells[_updatedCellCount++] = getCellLocation(x, y);
    }
}

void createGliderAt(unsigned char x, unsigned char y)
{
    addCell(x + 1, y);
    addCell(x + 2, y + 1);
    addCell(x, y + 2);
    addCell(x + 1, y + 2);
    addCell(x + 2, y + 2);
}

void main()
{
    clearScreen();
    printStr(6, 3, "Welcome to ZX Life!");
    _updatedCellCount = 0;
    createGliderAt(0, 0);
    createGliderAt(5, 5);
    createGliderAt(10, 10);
    createGliderAt(15, 15);
    while (1) {
        drawGrid();
        iterateGrid();
    }
}