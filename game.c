#include "screen.h"
#include "grid.h"

#define MAX_ACTIVE_CELLS 255

static unsigned int _updatedCellCount = 0;
static unsigned int _updatedCells[MAX_ACTIVE_CELLS];
static unsigned int _addedCells[MAX_ACTIVE_CELLS];
static unsigned int _activeCellCount = 0;
static unsigned int _activeCells[MAX_ACTIVE_CELLS];

static inline unsigned int getCellLocation(unsigned char x, unsigned char y)
{
    return ((int)y * GRID_WIDTH) + x + 1;
}

static inline unsigned char getCellXCoord(unsigned int cellLocation)
{
    return (cellLocation % GRID_WIDTH) - 1;
}

static inline unsigned char getCellYCoord(unsigned int cellLocation)
{
    return cellLocation / GRID_WIDTH;
}

static inline unsigned char isBitSet(unsigned char ch, unsigned char i)
{
    unsigned char mask = 1 << i;
    return mask & ch;
}

static inline unsigned char setBit(unsigned char ch, unsigned char i)
{
    unsigned char mask = 1 << i;
    return ch | mask;
}

static inline unsigned char clearBit(unsigned char ch, unsigned char i)
{
    unsigned char mask = 1 << i;
    mask = ~mask;
    return ch & mask;
}

void drawGrid(unsigned char ink)
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
            if (_addedCells[i] == cellLocation) {
                printBlockAt(x, y, ink); // added cells are bright blocks to distinguish them
            } else {
                printCellAt(x, y, ink);
            }
            gridValue = clearBit(gridValue, CELL_BORN);
        } else {
            clearCellAt(x, y);
            gridValue = clearBit(gridValue, CELL_DIES);            
        }
        setGridValue(x, y, gridValue);
        _addedCells[i] = 0;

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

static inline unsigned char wasCellAlive(unsigned char x, unsigned char y)
{
    unsigned char gridValue = getGridValue(x, y);
    if ((isBitSet(gridValue, CELL_ALIVE) && !isBitSet(gridValue, CELL_BORN)) || isBitSet(gridValue, CELL_DIES)) {
        return 1;
    }
    return 0;
}

static unsigned char updateCellState(unsigned char x, unsigned char y)
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

unsigned int iterateGrid()
{    
    // update grid and populate updated cells for draw method
    _updatedCellCount = 0;
    unsigned int i = 0;
    for (i = 0; i < _activeCellCount; i++) {
        unsigned int cellLocation = _activeCells[i];
        unsigned char x = getCellXCoord(cellLocation);
        unsigned char y = getCellYCoord(cellLocation);
        unsigned char gridValue = updateCellState(x, y);
        if ((isBitSet(gridValue, CELL_BORN) || isBitSet(gridValue, CELL_DIES)) && _updatedCellCount < MAX_ACTIVE_CELLS) {
            _updatedCells[_updatedCellCount++] = cellLocation;            
        }
    }

    return _updatedCellCount;
}

static inline void addCell(unsigned char x, unsigned char y)
{
    if (_updatedCellCount < MAX_ACTIVE_CELLS && x < GRID_WIDTH && y < GRID_HEIGHT) {
        unsigned char gridValue = setBit(0, CELL_ALIVE);
        gridValue = setBit(gridValue, CELL_BORN);
        setGridValue(x, y, gridValue);
        unsigned int cellLocation = getCellLocation(x, y);
        _updatedCells[_updatedCellCount] = cellLocation;
        _addedCells[_updatedCellCount++] = cellLocation;
    }
}

void drawChrAt(unsigned char x, unsigned char y, unsigned char c)
{
    unsigned char* glyph = getGlyphFromChr(c);
    unsigned char y2;
    unsigned char x2;
    for (y2 = 0; y2 < 8; y2++) {
        unsigned char row = *glyph++;
        for (x2 = 0; x2 < 8; x2++) {            
            if (row & 0x01) {
                addCell(x + 8 - x2, y + y2);
            }
            row = row >> 1;
        }
    }
}