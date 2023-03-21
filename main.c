#include <stdio.h>
#include <arch/zx.h>

#define GRID_WIDTH 31
#define GRID_HEIGHT 22
#define MAX_UPDATED_CELLS 50
#define MAX_ACTIVE_CELLS 200

// possible cell states stored in _grid
const unsigned char CELL_DEAD = 0; // cell was dead, still is dead
const unsigned char CELL_LIVES = 1; // cell was alive, still is alive
const unsigned char CELL_BORN = 2; // cell was dead, is now alive
const unsigned char CELL_DIES = 3; // cell was alive, is now dead
// adding this to a cell state marks it as active
const unsigned char CELL_ACTIVE = 4;

unsigned char _grid[GRID_WIDTH][GRID_HEIGHT];
unsigned int _updatedCellCount = 0;
unsigned int _updatedCells[MAX_UPDATED_CELLS];
unsigned int _activeCellCount = 0;
unsigned int _activeCells[MAX_ACTIVE_CELLS];

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

void drawGrid()
{
    _activeCellCount = 0;

    unsigned int i = 0;
    for (i = 0; i < _updatedCellCount; i++) {

        unsigned int cellLocation = _updatedCells[i];
        unsigned char x = getCellXCoord(cellLocation);
        unsigned char y = getCellYCoord(cellLocation);

        if (_grid[x][y] % CELL_ACTIVE == CELL_BORN) {
            printf("\x16\%c\%c0", x + 1, y + 1);
            _grid[x][y] = CELL_LIVES;
        } else if (_grid[x][y] % CELL_ACTIVE == CELL_DIES) {
            printf("\x16\%c\%c ", x + 1, y + 1);
            _grid[x][y] = CELL_DEAD;
        }

        int x2 = 0;
        int y2 = 0;
        for (x2 = x - 1; x2 <= x + 1; x2++) {
            if (x2 >= 0 && x2 < GRID_WIDTH) {
                for (y2 = y - 1; y2 <= y + 1; y2++) {
                    if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && _grid[x2][y2] < CELL_ACTIVE && _activeCellCount < MAX_ACTIVE_CELLS) {
                        _activeCells[_activeCellCount++] = getCellLocation(x2, y2);
                        _grid[x2][y2] += CELL_ACTIVE;
                    }
                }
            }
        }
    }
}

unsigned char wasCellAlive(unsigned char x, unsigned char y)
{
    if (_grid[x][y] % CELL_ACTIVE == CELL_LIVES || _grid[x][y] % CELL_ACTIVE == CELL_DIES) {
        return 1;
    }
    return 0;
}

unsigned char getCellState(unsigned char x, unsigned char y)
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
    if (wasAlive == 1 && total > 1 && total < 4) {
        return CELL_LIVES;
    } else if (wasAlive == 0 && total == 3) {
        return CELL_BORN;
    } else if (wasAlive == 0) {
        return CELL_DEAD;
    }
    return CELL_DIES;
}

void iterateGrid()
{
    _updatedCellCount = 0;

    unsigned int i = 0;
    for (i = 0; i < _activeCellCount; i++) {
        unsigned int cellLocation = _activeCells[i];
        unsigned char x = getCellXCoord(cellLocation);
        unsigned char y = getCellYCoord(cellLocation);
        _grid[x][y] = getCellState(x, y);
        if (_grid[x][y] == CELL_BORN || _grid[x][y] == CELL_DIES && _updatedCellCount < MAX_UPDATED_CELLS) {
            _updatedCells[_updatedCellCount++] = cellLocation;
        }
    }
}

void addCell(unsigned char x, unsigned char y)
{
    if (_updatedCellCount < MAX_UPDATED_CELLS) {
        _grid[x][y] = CELL_BORN;
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
    zx_cls(PAPER_WHITE);
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